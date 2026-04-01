#ifndef SPEEDCIRCULARLIST_SOLUTION_H
#define SPEEDCIRCULARLIST_SOLUTION_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
    int ans = 0;
    while (x != 1) {
        x /= 2;
        ++ans;
    }
    return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
        // Allowed to add members, but do not modify existing ones
        std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;

        Node(int bound_val, int fast_len) : bound(bound_val), next(nullptr), fast_search_list(nullptr) {
            if (fast_len > 0) {
                fast_search_list = new Node*[fast_len];
                for (int i = 0; i < fast_len; ++i) fast_search_list[i] = nullptr;
            }
        }

        ~Node() {
            if (fast_search_list) {
                delete[] fast_search_list;
                fast_search_list = nullptr;
            }
        }
    };

private:
    // Do not add more member variables
    Node* head = nullptr;
    int fast_search_list_size = 0;
    int list_size = 0;

    static int GetHashCode(std::string str) {
        long long ans = 0;
        for (auto &ch : str) {
            ans = (ans * s_prime + ch) % b_prime;
        }
        return static_cast<int>((ans + b_prime) % b_prime);
    }

    void BuildFastSearchList() {
        if (!head || list_size <= 0 || fast_search_list_size <= 0) return;

        // Set level 0: next pointers
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            if (cur->fast_search_list && fast_search_list_size > 0) {
                cur->fast_search_list[0] = cur->next;
            }
            cur = cur->next;
        }

        // Doubling: fast[k] = fast[k-1]->fast[k-1]
        for (int k = 1; k < fast_search_list_size; ++k) {
            cur = head;
            for (int i = 0; i < list_size; ++i) {
                if (cur->fast_search_list) {
                    Node* prev = cur->fast_search_list[k - 1];
                    cur->fast_search_list[k] = prev ? prev->fast_search_list[k - 1] : nullptr;
                }
                cur = cur->next;
            }
        }
    }

    // Helper: locate node for given code with linear scan
    Node* locate_node_by_code_linear(int code) const {
        if (!head) return nullptr;
        if (code <= head->bound) return head;
        Node* cur = head->next;
        // Since last bound == b_prime and code < b_prime, we must find a node
        while (cur != head) {
            if (code <= cur->bound) return cur;
            cur = cur->next;
        }
        // Fallback (should not reach here due to constraints)
        return head;
    }

public:
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = static_cast<int>(node_bounds.size());
        fast_search_list_size = (list_size <= 1) ? 0 : log2(list_size);
        if (list_size == 0) {
            head = nullptr;
            return;
        }

        // Create nodes
        std::vector<Node*> nodes;
        nodes.reserve(list_size);
        for (int b : node_bounds) {
            nodes.push_back(new Node(b, fast_search_list_size));
        }

        // Link circularly in increasing bound order (input guaranteed sorted)
        for (int i = 0; i < list_size; ++i) {
            nodes[i]->next = nodes[(i + 1) % list_size];
        }
        head = nodes[0];

        BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
        if (!head || list_size <= 0) return;
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = nullptr;
        list_size = 0;
        fast_search_list_size = 0;
    }

    void put(std::string str, T value) {
        int code = GetHashCode(str);
        // Please do not modify the line above
        Node* target = locate_node_by_code_linear(code);
        if (target) {
            target->kv_map[str] = value;
        }
    }

    T get(std::string str) {
        int code = GetHashCode(str);
        // Please do not modify the line above

        Node* target = locate_node_by_code_linear(code);
        if (target) {
            auto it = target->kv_map.find(str);
            if (it != target->kv_map.end()) return it->second;
        }
        return T();
    }

    void print() {
        if (!head) return;
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
            cur = cur->next;
        }
    }

    int size() const { return list_size; }
};

#endif // SPEEDCIRCULARLIST_SOLUTION_H

