#pragma once
#include <functional>

namespace Bifrost{
namespace Private {

void TBB_FOR_ALL(size_t start, size_t end, size_t block, std::function<void(size_t i)> function);
void TBB_FOR_ALL(size_t start, size_t end, size_t block, std::function<void(size_t start, size_t end)> function);

}} // Bifrost::Private
