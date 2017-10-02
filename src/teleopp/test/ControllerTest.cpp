
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <teleopp/ControllerToRobotPayload.h>
#include <teleopp/TeleoppController.h>

#include <chrono>

using namespace teleopp;
using namespace testing;

namespace {

void EXPECT_CONTROLLER_STATE_UKNOWN_ROBOT(const Document& expectedDocument,
                                          const ControllerState& testState) {
  EXPECT_THAT(testState.latestDocument, Pointee(Eq(expectedDocument)));
  EXPECT_FALSE(testState.knownRobotState);
  EXPECT_THAT(testState.overrideReasons, Contains(RobotOverrideReason::ROBOT_GONE_ROGUE));
}

void EXPECT_NET_PAYLOAD_UNKNOWN_ROBOT(const Document& expectedDocument,
                                      uint64_t expectedControllerVersion,
                                      Time expectedControllerTime,
                                      NetPayloadPtr testPayload) {
  EXPECT_THAT(controller_to_robot::copyDocument(testPayload), Pointee(Eq(expectedDocument)));
  EXPECT_EQ(expectedControllerVersion, controller_to_robot::controllerVersion(testPayload));
  EXPECT_EQ(expectedControllerTime, controller_to_robot::controllerVersionTime(testPayload));
  EXPECT_EQ(0, controller_to_robot::basisRobotVersion(testPayload));
}

TEST(ControllerTest, makeController) {
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), "abc");
}

TEST(ControllerTest, initialControllerState) {
  Document initialDocument = "abc";
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), initialDocument);

  EXPECT_CONTROLLER_STATE_UKNOWN_ROBOT(initialDocument, controller.checkState());
}

TEST(ControllerTest, firstTick) {
  auto startTime = std::chrono::steady_clock::now();
  Millis nominalTick(50);
  auto firstTickTime = std::chrono::time_point_cast<Time::duration>(startTime + nominalTick);

  Document firstTickDocument = "def";
  TeleoppController controller(nominalTick, startTime, "abc");
  auto tickAction = controller.tick(firstTickTime, firstTickDocument);

  EXPECT_CONTROLLER_STATE_UKNOWN_ROBOT(firstTickDocument, tickAction.controllerState);
  EXPECT_NET_PAYLOAD_UNKNOWN_ROBOT(firstTickDocument, 1, firstTickTime, tickAction.outboundPayload);
}

TEST(ControllerTest, manyTicks) {
  Time curTime = std::chrono::time_point_cast<Time::duration>(Time::clock::now());
  Millis nominalTick(50);

  TeleoppController controller(nominalTick, curTime, "abc");

  for (uint64_t i = 1; i <= 50; i++) {
    std::ostringstream oss;
    oss << i;
    Document doc = oss.str();
    curTime += nominalTick;
    auto tickAction = controller.tick(curTime, doc);

    EXPECT_CONTROLLER_STATE_UKNOWN_ROBOT(doc, tickAction.controllerState);
    EXPECT_NET_PAYLOAD_UNKNOWN_ROBOT(doc, i, curTime, tickAction.outboundPayload);
  }
}
} // namespace
