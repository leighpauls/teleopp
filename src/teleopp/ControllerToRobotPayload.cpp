#include "ControllerToRobotPayload.h"

#include <teleopp_fbs/Teleopp_generated.h>

namespace teleopp {

NetPayload controller_to_robot::build(DocumentPtr document,
                                      uint64_t controllerVersion,
                                      Time controllerVersionTime,
                                      uint64_t basisRobotVersion) {
  flatbuffers::FlatBufferBuilder fbb;
  fbb.Finish(CreateTeleoppMessage(
      fbb,
      TeleoppUnion_ControllerToRobotPayload,
      CreateControllerToRobotPayload(
          fbb,
          fbb.CreateVector(reinterpret_cast<const uint8_t*>(document->data()), document->size()),
          controllerVersion,
          Millis(controllerVersionTime.time_since_epoch()).count(),
          basisRobotVersion)
          .Union()));
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
  if (message->teleoppUnion_type() != TeleoppUnion_ControllerToRobotPayload) {
    throw std::runtime_error("Expected ControllerToRobotPayload");
  }
  return message->teleoppUnion_as_ControllerToRobotPayload();
}
} // namespace

DocumentPtr controller_to_robot::copyDocument(NetPayloadPtr payload) {
  auto payloadVector = asFlatbuffer(payload)->newControlDocument();
  return std::make_shared<Document>(reinterpret_cast<const char*>(payloadVector->data()),
                                    payloadVector->size());
}

uint64_t controller_to_robot::controllerVersion(NetPayloadPtr payload) {
  return asFlatbuffer(payload)->controllerVersion();
}

Time controller_to_robot::controllerVersionTime(NetPayloadPtr payload) {
  return Time(Millis(asFlatbuffer(payload)->controllerVersionTimeMs()));
}

uint64_t controller_to_robot::basisRobotVersion(NetPayloadPtr payload) {
  return asFlatbuffer(payload)->basisRobotVersion();
}

} // namespace teleopp
