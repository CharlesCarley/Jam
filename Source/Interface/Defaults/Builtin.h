#pragma once
#include <vector>
namespace Jam::Builtin
{
   using ByteArray = std::vector<uint8_t>;


   class Layouts
   {
   public:
       static void getStack(ByteArray &dest);

       static void getLayout(ByteArray &dest);
   };
} // namespace Jam::Builtin
