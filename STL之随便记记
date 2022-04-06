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
class Solution {
public:
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
};
