#pragma once

#include "CommonTypes.h"

namespace teleopp {

struct controller_to_robot {
  static NetPayload build(DocumentPtr document,
                          uint64_t controllerVersion,
                          Time controllerVersionTime,
                          uint64_t basisRobotVersion);

  static DocumentPtr copyDocument(NetPayloadPtr payload);
  static uint64_t controllerVersion(NetPayloadPtr payload);
  static Time controllerVersionTime(NetPayloadPtr payload);
  static uint64_t basisRobotVersion(NetPayloadPtr payload);
};
}
