lower_bound( )和upper_bound( )的常见用法

一、在从小到大的排序数组中，

lower_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于或等于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。

upper_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。


二、在从大到小的排序数组中，重载lower_bound()和upper_bound()

lower_bound( begin,end,num,greater<type>() ):从数组的begin位置到end-1位置二分查找第一个小于或等于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。

upper_bound( begin,end,num,greater<type>() ):从数组的begin位置到end-1位置二分查找第一个小于num的数字，找到返回该数字的地址，不存在则返回end。
通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。

例：
    
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
    
        sort(arr2.begin(), arr2.end());
        int len = (int)arr2.size();
        int cnt = 0, index;
        
        for(int i : arr1) {
            index = lower_bound(arr2.begin(), arr2.end(), i) - arr2.begin();
            if(index >= 0 && index < len) {
                if(abs(arr2[index] - i) <= d) {
                    continue;
                }
            }
            index --;
            if(index >= 0 && index < len) {
                if(abs(arr2[index] - i) <= d) {
                    continue;
                }
            }
            cnt ++;
        }
        return cnt;
    }
 
   
map和unordered_map的差别
    
需要引入的头文件不同
map: #include < map >
unordered_map: #include < unordered_map >

内部实现机理不同
map： map内部实现了一个红黑树（红黑树是非严格平衡二叉搜索树，而AVL是严格平衡二叉搜索树），红黑树具有自动排序的功能，因此map内部的所有元素都是有序的，红黑树的每一个节点都代表着map的一个元素。因此，对于map进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行的操作。map中的元素是按照二叉搜索树（又名二叉查找树、二叉排序树，特点就是左子树上所有节点的键值都小于根节点的键值，右子树所有节点的键值都大于根节点的键值）存储的，使用中序遍历可将键值按照从小到大遍历出来。
    
unordered_map: unordered_map内部实现了一个哈希表（也叫散列表，通过把关键码值映射到Hash表中一个位置来访问记录，查找的时间复杂度可达到O(1)，其在海量数据处理中有着广泛应用）。因此，其元素的排列顺序是无序的
    
    
