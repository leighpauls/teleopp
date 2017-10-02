
#include "RobotToControllerAck.h"

#include <teleopp_fbs/Teleopp_generated.h>

namespace teleopp {

NetPayload robot_to_controller_ack::build(uint64_t controllerVersion, uint64_t robotVersion) {
  flatbuffers::FlatBufferBuilder fbb;
  fbb.Finish(CreateTeleoppMessage(
      fbb,
      TeleoppUnion_RobotToControllerAck,
      CreateRobotToControllerAck(fbb, controllerVersion, robotVersion).Union()));

  // TODO: I can release and move fbb's internal buffer
  return NetPayload(reinterpret_cast<const char*>(fbb.GetBufferPointer()), fbb.GetSize());
}

namespace {
auto asFlatbuffer(NetPayloadPtr payload) {
  {
    // TODO: this only needs to be done once per NetPayload instance
    flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(payload->data()),
                                   payload->size());
    if (!VerifyTeleoppMessageBuffer(verifier)) {
      throw std::runtime_error("Invalid TeleoppMessage");
    }
  }
  auto message = GetTeleoppMessage(payload->data());
  if (message->teleoppUnion_type() != TeleoppUnion_RobotToControllerAck) {
    throw std::runtime_error("Expected RobotToControllerAck");
  }
  return message->teleoppUnion_as_RobotToControllerAck();
}

} // namespace

uint64_t robot_to_controller_ack::controllerVersion(NetPayloadPtr payload) {
  return asFlatbuffer(payload)->controllerVersion();
}

uint64_t robot_to_controller_ack::robotVersion(NetPayloadPtr payload) {
  return asFlatbuffer(payload)->robotVersion();
}
} // namespace teleopp
