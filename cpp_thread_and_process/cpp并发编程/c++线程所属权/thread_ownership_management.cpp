/* 
 * Description: c++ thread ownership management demo
 * Create: 2024-06-13
 * Owner: 
 */

#include "thread_ownership_management.h"

namespace ThreadOwnershipManagement  {

} // namespace ThreadOwnershipManagement

int main()
{   
    // ThreadOwnershipManagement::DangerousUse();
    // ThreadOwnershipManagement::UseJoinThread();
    // ThreadOwnershipManagement::UseVector();
    ThreadOwnershipManagement::UseParallelAcc();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
