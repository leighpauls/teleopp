
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <teleopp/ControllerToRobotPayload.h>
#include <teleopp/RobotToControllerAck.h>
#include <teleopp_fbs/Teleopp_generated.h>

#include <chrono>

using namespace teleopp;
using namespace testing;

namespace {

TEST(RobotToControllerAckTest, writeRead) {
  uint64_t controllerVersion = 1234;
  uint64_t robotVersion = 5678;

  NetPayloadPtr payload;
  // Use scope to ensure all original pointers are destroyed before reading
  {
    payload = std::make_shared<NetPayload>(
        robot_to_controller_ack::build(controllerVersion, robotVersion));
  }

  EXPECT_EQ(controllerVersion, robot_to_controller_ack::controllerVersion(payload));
  EXPECT_EQ(robotVersion, robot_to_controller_ack::robotVersion(payload));
}


TEST(RobotToControllerAckTest, rejectInvalid) {
  EXPECT_ANY_THROW(
      { robot_to_controller_ack::controllerVersion(std::make_shared<NetPayload>("poop")); });
}

TEST(RobotToControllerAckTest, rejectWrongMessageType) {
  EXPECT_ANY_THROW({
    robot_to_controller_ack::controllerVersion(std::make_shared<NetPayload>(
        controller_to_robot::build(std::make_shared<Document>("poop"),
                                   123,
                                   std::chrono::time_point_cast<Time::duration>(Time::clock::now()),
                                   456)));
  });
}
} // namespace
