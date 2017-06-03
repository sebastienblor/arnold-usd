#pragma once

#include <aminomath/bbox.h>
#include <bifrostapi/bifrost_layout.h>

namespace Bifrost{
namespace Processing{

amino::Math::bboxf computeBBox(const Bifrost::API::Layout& layout);

void optimize(Bifrost::API::Layout layout);

}}
