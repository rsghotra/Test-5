#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct ListNode {
  ListNode* next;
  int data;
  ListNode(int d) {
    this->data = d;
    this->next = 0;
  }
};

bool IfPermutationExists(const string& str, const string& pattern) {
  unordered_map<char, int> frequencies;
  for(char ch: pattern) {
    frequencies[ch]++;
  }

  int windowStart = 0;
  int matchCount = 0;

  for(int windowEnd = 0; windowEnd < str.size(); windowEnd++) {
    if(frequencies.find(str[windowEnd]) != frequencies.end()) {
      frequencies[str[windowEnd]]--;
      if(frequencies[str[windowEnd]] == 0) {
        matchCount++;
      }
    }
    if(matchCount == frequencies.size()) {
      return true;
    }

    if(windowEnd - windowStart + 1 == pattern.size()) {
      if(frequencies.find(str[windowStart]) != frequencies.end()) {
        if(frequencies[str[windowStart]]== 0){
          matchCount--;
        }
        frequencies[str[windowStart]]++;
      }
      windowStart++;
    }
  }
  return false;
}

void IfPermutationExists() {
  cout << "Permutation exist: " << IfPermutationExists("oidbcaf", "abc") << endl;
  cout << "Permutation exist: " << IfPermutationExists("odicf", "dc") << endl;
  cout << "Permutation exist: " << IfPermutationExists("bcdxabcdy", "bcdyabcdx") << endl;
  cout << "Permutation exist: " << IfPermutationExists("aaacb", "abc") << endl;
}

void SearchPairs(vector<int>& nums, int target, int first, int second, vector<vector<int>>& quads) {
  int left = second + 1;
  int right = nums.size() - 1;
  target = target - nums[first] - nums[second]; //inorder to avoid int overflow;
  while(left < right) {
    if(nums[left] + nums[right] == target) {
      quads.push_back({nums[first], nums[second], nums[left], nums[right]});
      left++;
      right--;
      while(left < right && nums[left-1] == nums[left]) {
        left++;
      }

      while(left < right && nums[right-1] == nums[right]) {
        right--;
      }
    } else if(nums[left] + nums[right] < target) {
      left++;
    } else {
      right--;
    }
  }
}

vector<vector<int>> FindQuadruplets(vector<int>& nums, int target) {
  vector<vector<int>> quadruplets;
  for(int i = 0; i < nums.size() - 3; i++) {
    if(i > 0 && nums[i-1] == nums[i]) {
      continue;
    }
    for(int j = i+1; j < nums.size() - 2; j++) {
      if(j > i && nums[j-1] == nums[j]) {
        continue;
      }
      SearchPairs(nums, target, i, j, quadruplets);
    }
  }
  return quadruplets;
}

void FindQuadruplets() {
  vector<int> vec = {4, 1, 2, -1, 1, -3};
  auto result = FindQuadruplets(vec, 1);
  for (auto vec : result) {
    cout << "[";
    for (auto num : vec) {
      cout << num << " ";
    }
    cout << "]";
  }
  cout << endl;

  vec = {2, 0, -1, 1, -2, 2};
  result = FindQuadruplets(vec, 2);
  for (auto vec : result) {
    cout << "[";
    for (auto num : vec) {
      cout << num << " ";
    }
    cout << "]";
  }
}

ListNode* Reverse(ListNode* head) {
  ListNode* current = head;
  ListNode* previous = 0;
  while(current != 0) {
    ListNode* next = current->next;
    current->next = previous;
    previous  = current;
    current = next;
  }

  return previous;
}

bool IsPalindrome(ListNode* head) {
  //find middle; reverse; compare; reset back the LL
  if(head == 0 || head->next == 0) return true;

  ListNode* slow = head;
  ListNode* fast = head;
  while(fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  ListNode* reversedHead = Reverse(slow);
  ListNode* copyReversedHead = reversedHead;

  ListNode* current = head;
  while(current != 0 && reversedHead != 0) {
    if(current->data != reversedHead->data) {
      break;
    }
    current = current->next;
    reversedHead = reversedHead->next;
  }
  Reverse(copyReversedHead); //reset back
  if(current == 0 || reversedHead == 0) {
    return true;
  }
  return false;
}

void IsPalindrome() {
  ListNode *head = new ListNode(2);
  head->next = new ListNode(4);
  head->next->next = new ListNode(6);
  head->next->next->next = new ListNode(4);
  head->next->next->next->next = new ListNode(2);
  cout << "\nIs palindrome: " << IsPalindrome(head) << endl;

  head->next->next->next->next->next = new ListNode(2);
  cout << "Is palindrome: " << IsPalindrome(head) << endl;
}

struct EndCompare {
  bool operator()(const vector<int>& x, const vector<int>& y) {
    return x[1] > y[1];
  }
};

int MinMeetingRooms(vector<vector<int>>& meetings) {
  //sort meetings by start time;
  if(meetings.empty()) return 0;
  std::sort(meetings.begin(), meetings.end(), [](const vector<int>& m1, const vector<int>& m2) {
    return m1[0] < m2[0];
  });

  priority_queue<vector<int>, vector<vector<int>>, EndCompare> minHeap;
  int minMeetingRooms = 0;


  for(auto meeting: meetings) {
    while(!minHeap.empty() && meeting[0] >= minHeap.top()[1]) {
      minHeap.pop();
    }
    minHeap.push(meeting);
    //all active meetings are in the heap so we need the height
    minMeetingRooms = max(minMeetingRooms, (int)minHeap.size());
  }
  return minMeetingRooms;
}

void MinMeetingRooms() {
  vector<vector<int>> input = {{4, 5}, {2, 3}, {2, 4}, {3, 5}};
  int result = MinMeetingRooms(input);
  cout << "\nMinimum meeting rooms required: " << result << endl;

  input = {{1, 4}, {2, 5}, {7, 9}};
  result = MinMeetingRooms(input);
  cout << "Minimum meeting rooms required: " << result << endl;

  input = {{6, 7}, {2, 4}, {8, 12}};
  result = MinMeetingRooms(input);
  cout << "Minimum meeting rooms required: " << result << endl;

  input = {{1, 4}, {2, 3}, {3, 6}};
  result = MinMeetingRooms(input);
  cout << "Minimum meeting rooms required: " << result << endl;

  input = {{4, 5}, {2, 3}, {2, 4}, {3, 5}};
  result = MinMeetingRooms(input);
  cout << "Minimum meeting rooms required: " << result << endl;
}

ListNode* ReverseAlternatingKLL(ListNode* head, int K) {
  if(K <= 1 || head == 0) return head;
  
  ListNode* current = head;
  ListNode* previous = 0;

  while(current != 0) {
    ListNode* lastPrevious = previous;
    ListNode* lastCurrent = current;

    for(int i = 0; i < K && current != 0; i++) {
      ListNode* next = current->next;
      current->next = previous;
      previous = current;
      current = next;
    }

    if(lastPrevious != 0) {
      lastPrevious->next = previous;
    } else {
      head = previous;
    }

    lastCurrent->next = current;
    if(current == 0) {
      break;
    }

    previous = lastCurrent; // not necessary to do it as we are overwriting in line 252
    for(int j = 0; j < K && current != 0; j++) {
      previous = current;
      current = current->next;
    }
  }
  return head;
}

void ReverseAlternatingKLL() {
  ListNode *head = new ListNode(1);
  head->next = new ListNode(2);
  head->next->next = new ListNode(3);
  head->next->next->next = new ListNode(4);
  head->next->next->next->next = new ListNode(5);
  head->next->next->next->next->next = new ListNode(6);
  head->next->next->next->next->next->next = new ListNode(7);
  head->next->next->next->next->next->next->next = new ListNode(8);

  ListNode *result = ReverseAlternatingKLL(head, 2);
  cout << "\nNodes of the reversed LinkedList are: ";
  while (result != nullptr) {
    cout << result->data << " ";
    result = result->next;
  }
}

int main() {
  IfPermutationExists();
  FindQuadruplets();
  IsPalindrome();
  ReverseAlternatingKLL();
  MinMeetingRooms();
  return 0;
}