
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <teleopp/ControllerToRobotPayload.h>
#include <teleopp_fbs/Teleopp_generated.h>

#include <chrono>

using namespace teleopp;
using namespace testing;

namespace {

TEST(ControllerToRobotPayloadTest, writeRead) {

  Document document = "abcd";
  uint64_t controllerVersion = 1234;
  Time controllerVersionTime = std::chrono::time_point_cast<Time::duration>(Time::clock::now());
  uint64_t basisRobotVersion = 5678;

  NetPayloadPtr payload;
  // Use scope to ensure all original pointers are destroyed before reading
  {
    payload = std::make_shared<NetPayload>(
        controller_to_robot::build(std::make_shared<Document>(document),
                                   controllerVersion,
                                   controllerVersionTime,
                                   basisRobotVersion));
  }

  EXPECT_THAT(controller_to_robot::copyDocument(payload), Pointee(Eq(document)));
  EXPECT_EQ(controllerVersion, controller_to_robot::controllerVersion(payload));
  EXPECT_EQ(controllerVersionTime, controller_to_robot::controllerVersionTime(payload));
  EXPECT_EQ(basisRobotVersion, controller_to_robot::basisRobotVersion(payload));
}
} // namespace
