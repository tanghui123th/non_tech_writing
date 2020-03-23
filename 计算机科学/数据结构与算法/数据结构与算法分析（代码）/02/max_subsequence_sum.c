#include <stdio.h>
#include <math.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

/*
 * 最大子列和1
 * O(n^2)时间复杂度
*/
int MaxSubSequenceSum1(const int A[], int N)
{
    int this_sum, max_sum, i, j;

    max_sum = 0;
    for (int i = 0; i < N; i++) {
        this_sum = 0;
        for (int j = i; j < N; j++) {
            this_sum += A[j];
            if (max_sum < this_sum)
                max_sum = this_sum;
        }
    }
    return max_sum;
}

/*
 * 最大子列和2
 * O(n*logn)时间复杂度
 * 
 * 最大子列和可能在三种地方出现：
 * 1. 左半部分
 * 2. 右半部分
 * 3. 跨越数据的中部。这种情况要求必须把center和center+1位置的数算进去。否则就退化成1和2了
 * 
 * 分成两半部分递归复杂度是O(logn)，但是每个递归实例里还有跨越数据的中部，复杂度是O(n)
 * 因此总时间复杂度就是O(n*logn)
*/

int MaxSubSum(const int A[], int left, int right)
{
    /* 左闭右闭 [left, right] */
    int max_left_sum, max_right_sum;
    int max_left_border_sum, max_right_border_sum;
    int left_border_sum, right_border_sum;
    int center, i;

    if (left == right)    /* base case */
        return A[left] > 0 ? A[left] : 0;

    center = (left + right) / 2;
    max_left_sum = MaxSubSum(A, left, center);
    max_right_sum = MaxSubSum(A, center + 1, right);

    max_left_border_sum = 0, left_border_sum = 0;
    for (int i = center; left <= i; i--) {
        left_border_sum += A[i];
        max_left_border_sum = max(max_left_border_sum, left_border_sum);
    }

    max_right_border_sum = 0, right_border_sum = 0;
    for (int i = center + 1; i <= right; i++) {
        right_border_sum += A[i];
        max_right_border_sum = max(max_right_border_sum, right_border_sum);
    }

    return max(max_left_border_sum + max_right_border_sum, max(max_left_sum, max_right_sum));
}

int MaxSubSequenceSum2(const int A[], int N)
{
    return MaxSubSum(A, 0, N - 1);
}

/*
 * 最大子列和3
 * O(n)时间复杂度
*/

int MaxSubSequenceSum3(const int A[], int N)
{
    int this_sum, max_sum;

    this_sum = max_sum = 0;
    for (int i = 0; i < N; i++) {
        this_sum += A[i];
        this_sum = max(this_sum, 0);
        max_sum = max(max_sum, this_sum);
    }
    return max_sum;
}

void test(const char* test_name, const int A[], int N, int expected)
{
    int res1 = MaxSubSequenceSum1(A, N);
    if (res1 == expected) {
        printf("%s MaxSubSequenceSum1 success.\n", test_name);
    } else {
        printf("%s MaxSubSequenceSum1 failed.\n", test_name);
    }

    int res2 = MaxSubSequenceSum2(A, N);
    if (res2 == expected) {
        printf("%s MaxSubSequenceSum2 success.\n", test_name);
    } else {
        printf("%s MaxSubSequenceSum2 failed.\n", test_name);
    }

    int res3 = MaxSubSequenceSum3(A, N);
    if (res3 == expected) {
        printf("%s MaxSubSequenceSum3 success.\n", test_name);
    } else {
        printf("%s MaxSubSequenceSum3 failed.\n", test_name);
    }
}

int main()
{
    const int A1[] = {4, -3, 5, -2, -1, 2, 6, -2};
    int N1 = 8;
    int expected1 = 11;
    test("test1", A1, N1, expected1);

    return 0;
}
