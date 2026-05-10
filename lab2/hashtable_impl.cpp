/*
*  DKU Operating System Lab (2026)
*      Lab2 (Concurrency Data Structure: Hash Table)
*      Student id : 32213679
*      Student name : 이현서
*      Date: 2026/05/10
*/


#include "hashtable_impl.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

// DefaultHashTable
// DefaultHashTable 생성자
DefaultHashTable::DefaultHashTable(int num_buckets) : num_buckets_(num_buckets) {
    // 버킷 배열 할당 및 초기화 (모두 nullptr)
    buckets_ = new HTNode*[num_buckets_];
    for (int i = 0; i < num_buckets_; i++) {
        buckets_[i] = nullptr;
    }
}

// DefaultHashTable 소멸자
DefaultHashTable::~DefaultHashTable() {
    // 각 버킷의 체인 노드 전부 해제
    for (int i = 0; i < num_buckets_; i++) {
        HTNode* node = buckets_[i];
        while (node) {
            HTNode* tmp = node->next;
            delete node;
            node = tmp;
        }
    }
    delete[] buckets_;
}

// 해시 함수 : key를 버킷 수로 나눈 나머지
int DefaultHashTable::hash_func(int key) {
    return key % num_buckets_;
}

// 순회 함수 : 전체 버킷을 순회하여 key 오름차순으로 정렬 후 arr에 저장
// Hash Table은 삽입 순서를 보장하지 않으므로, std::sort로 정렬한다.
void DefaultHashTable::traversal(KVC* arr) {
    std::vector<KVC> tmp;
    for (int i = 0; i < num_buckets_; i++) {
        HTNode* node = buckets_[i];
        while (node) {
            KVC kvc;
            kvc.key     = node->key;
            kvc.value   = node->value;
            kvc.upd_cnt = node->upd_cnt;
            tmp.push_back(kvc);
            node = node->next;
        }
    }
    // key 기준 오름차순 정렬 (std::map 순회와 동일한 순서 보장)
    std::sort(tmp.begin(), tmp.end(), [](const KVC& a, const KVC& b) {
        return a.key < b.key;
    });
    for (int i = 0; i < (int)tmp.size(); i++) {
        arr[i] = tmp[i];
    }
}

// HashTable (without lock)
// HashTable 생성자
HashTable::HashTable(int num_buckets) : DefaultHashTable(num_buckets) {}

// HashTable 소멸자
HashTable::~HashTable() {}

void HashTable::insert(int key, int value) {

    // 해시 함수로 버킷 인덱스 구함
    int index = hash_func(key);

    // 연결 리스트 첫번째 노드 가리킴
    HTNode* curr = buckets_[index];

    // 연결 리스트에 동일 키 존재하는지 확인
    while (curr != nullptr) {
        // 이미 키가 존재하면 value, upd_cnt 증가
        if (curr->key == key) {
            curr->value += value;
            curr->upd_cnt++;
            return;
        }
        curr = curr->next;
    }

    // 동일한 키가 없는 경우 새 노드 생성
    HTNode* new_node = new HTNode;
    new_node->key = key;
    new_node->upd_cnt = 0;
    new_node->value = value;

    // 새 노드를 연결 리스트 맨 앞으로
    new_node->next = buckets_[index];   // 기존 첫 노드를 새 노드 다음으로 포인터 설정
    buckets_[index] = new_node;         // 버킷 시작점을 새 노드로 설정
}

int HashTable::lookup(int key) {

    // 해시 함수로 버킷 인덱스 구함
    int index = hash_func(key);

    // 버킷 인덱스로 탐색
    HTNode* curr = buckets_[index];

    while (curr != nullptr) {
        // key 값 일치하면 value 반환
        if (curr->key == key)  {
            return curr->value;
        }
        // 다음 노드로 이동 
        curr = curr->next;
    }
    return 0;   // value가 없으면 0 반환
}

void HashTable::remove(int key) {
    // 구현
    int index = hash_func(key);
    HTNode* curr = buckets_[index];
    HTNode* prev = nullptr;             // 맨 앞 노드는 이전 노드 존재 x -> nullptr

    while (curr != nullptr) {
        if (prev == nullptr) {
            // 첫 노드를 삭제하는 경우
            buckets_[index] = curr->next;
        } else {
        // 중간 노드 삭제
        prev->next = curr->next;
        }
    // 메모리 누수 방지 위해 메모리 해제
    delete curr;
    return;
    }
    // 다음 노드로 이동
    prev = curr;
    curr = curr->next;
}

void HashTable::traversal(KVC* arr) {
    // 구현
}

// CoarseHashTable (coarse-grained lock)
// CoarseHashTable 생성자
CoarseHashTable::CoarseHashTable(int num_buckets) : DefaultHashTable(num_buckets) {
    pthread_mutex_init(&mutex_lock, nullptr);
}

// CoarseHashTable 소멸자
CoarseHashTable::~CoarseHashTable() {
    pthread_mutex_destroy(&mutex_lock);
}

void CoarseHashTable::insert(int key, int value) {
    // 구현
}

int CoarseHashTable::lookup(int key) {
    // 구현
}

void CoarseHashTable::remove(int key) {
    // 구현
}

void CoarseHashTable::traversal(KVC* arr) {
    // 구현
}

// FineHashTable (fine-grained lock)
// FineHashTable 생성자
FineHashTable::FineHashTable(int num_buckets) : DefaultHashTable(num_buckets) {
    // 버킷 수만큼 뮤텍스 배열 할당 및 초기화
    bucket_locks = new pthread_mutex_t[num_buckets_];
    for (int i = 0; i < num_buckets_; i++) {
        pthread_mutex_init(&bucket_locks[i], nullptr);
    }
}

// FineHashTable 소멸자
FineHashTable::~FineHashTable() {
    for (int i = 0; i < num_buckets_; i++) {
        pthread_mutex_destroy(&bucket_locks[i]);
    }
    delete[] bucket_locks;
}

void FineHashTable::insert(int key, int value) {
    // 구현
}

int FineHashTable::lookup(int key) {
    // 구현
}

void FineHashTable::remove(int key) {
    // 구현
}

void FineHashTable::traversal(KVC* arr) {
    // 구현
}
