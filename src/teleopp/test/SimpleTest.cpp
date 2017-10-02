
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <teleopp/TeleoppController.h>

#include <chrono>

using namespace teleopp;
using namespace testing;

namespace {

TEST(SimpleTest, makeController) {
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), "abc");
}

TEST(SimpleTest, initialControllerState) {
  Document initialDocument = "abc";
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), initialDocument);
  auto initialState = controller.checkState();
  EXPECT_THAT(initialState.latestDocument, Pointee(Eq(initialDocument)));
  EXPECT_FALSE(initialState.robotPastState);
  EXPECT_THAT(initialState.overrideReasons, Contains(RobotOverrideReason::ROBOT_GONE_ROGUE));
}

} // namespace
