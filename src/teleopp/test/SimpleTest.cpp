
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <teleopp/TeleoppController.h>
#include <teleopp_fbs/Teleopp_generated.h>

#include <chrono>

using namespace teleopp;
using namespace testing;

namespace {

TEST(SimpleTest, makeCointroller) {
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), "abc");
}

TEST(SimpleTest, initialControllerState) {
  Document initialDocument = "abc";
  TeleoppController controller(Millis(50), std::chrono::steady_clock::now(), initialDocument);
  auto initialState = controller.checkState();
  ASSERT_THAT(initialState.latestDocument, Pointee(Eq(initialDocument)));
  ASSERT_FALSE(initialState.robotPastState);
  ASSERT_THAT(initialState.overrideReasons, Contains(RobotOverrideReason::ROBOT_GONE_ROGUE));
}

} // namespace
