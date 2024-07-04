/* 
 * Description: c++ thread mutually exclusive and deadlock
 * Create: 2024-06-20
 * Owner: 
 */

#include "lock_mutex.h"

int main() {
    // LockMutex::TestLock();
    // LockMutex::TestSafeSwap();
    LockMutex::TestHierarchyLock();
    return 0;
}
