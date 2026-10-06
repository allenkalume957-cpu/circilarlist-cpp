#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

class CircularList {
private:
    struct Node {
        int value;
        Node* next;
        explicit Node(int value) : value(value), next(nullptr) {}
    };

    Node* tail = nullptr;  // tail->next is the head (circular link)
    std::size_t count = 0;

public:
    CircularList() = default;

    // Rule of five: no copying (raw owning pointer)
    CircularList(const CircularList&) = delete;
    CircularList& operator=(const CircularList&) = delete;

    ~CircularList() { clear(); }

    bool empty() const { return tail == nullptr; }
    std::size_t size() const { return count; }

    Node* head() const { return empty() ? nullptr : tail->next; }

    // Insert at the front: O(1)
    void pushFront(int value) {
        Node* node = new Node(value);
        if (empty()) {
            node->next = node;   // single node points to itself
            tail = node;
        } else {
            node->next = tail->next;
            tail->next = node;
        }
        ++count;
    }

    // Insert at the back: O(1) thanks to the tail pointer
    void pushBack(int value) {
        Node* node = new Node(value);
        if (empty()) {
            node->next = node;
            tail = node;
        } else {
            node->next = tail->next;
            tail->next = node;
            tail = node;
        }
        ++count;
    }

    // Remove from the front: O(1)
    std::optional<int> popFront() {
        if (empty()) return std::nullopt;

        Node* first = tail->next;
        int value = first->value;

        if (first == tail) {        // last element
            tail = nullptr;
        } else {
            tail->next = first->next;
        }
        delete first;
        --count;
        return value;
    }

    // Remove the first node holding `value`: O(n)
    bool removeValue(int value) {
        if (empty()) return false;

        Node* prev = tail;
        Node* curr = tail->next;    // head

        for (std::size_t i = 0; i < count; ++i) {
            if (curr->value == value) {
                if (count == 1) {
                    tail = nullptr;              // removing the only node
                } else if (curr == tail) {
                    prev->next = curr->next;     // unlink the tail first
                    tail = prev;                 // prev is now the last node
                } else {
                    prev->next = curr->next;     // unlink a middle/front node
                }
                delete curr;
                --count;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // Linear search: O(n)
    std::optional<int> find(int value) const {
        if (empty()) return std::nullopt;
        Node* curr = tail->next;
        for (std::size_t i = 0; i < count; ++i) {
            if (curr->value == value) return static_cast<int>(i);
            curr = curr->next;
        }
        return std::nullopt;
    }

    // Walk the circle (exactly `count` steps, so it always terminates)
    std::string toString() const {
        if (empty()) return "(empty)";
        std::ostringstream out;
        Node* curr = tail->next;
        for (std::size_t i = 0; i < count; ++i) {
            if (i) out << " -> ";
            out << curr->value;
            curr = curr->next;
        }
        out << " -> (back to " << tail->next->value << ")";
        return out.str();
    }

    void display(const std::string& label = "List") const {
        std::cout << std::left << std::setw(17) << label << ": " << toString()
                  << "   [size=" << count << "]\n";
    }

    void clear() {
        if (empty()) return;
        Node* curr = tail->next;
        for (std::size_t i = 0; i < count; ++i) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        tail = nullptr;
        count = 0;
    }
};

int main() {
    std::cout << "===== Circular List Demo =====\n\n";

    CircularList list;
    list.display("Start");

    // Build the list from the back
    for (int value : {10, 20, 30, 40}) {
        list.pushBack(value);
        list.display("pushBack(" + std::to_string(value) + ")");
    }

    std::cout << "\n";

    // Insert at the front
    list.pushFront(5);
    list.display("pushFront(5)");

    // Search
    if (auto pos = list.find(30)) {
        std::cout << "find(30)       -> found at index " << *pos << "\n";
    }
    if (auto pos = list.find(99); !pos) {
        std::cout << "find(99)       -> not found\n";
    }

    std::cout << "\n";

    // Remove from the front
    if (auto v = list.popFront()) {
        std::cout << "popFront()     -> removed " << *v << "\n";
    }
    list.display("after popFront");

    // Remove a value in the middle
    std::cout << "removeValue(20) -> "
              << (list.removeValue(20) ? "removed 20" : "not found") << "\n";
    list.display("after remove");

    std::cout << "\n";

    // Circularity check: walk one full lap
    std::cout << "Full lap       : ";
    const std::size_t lap = list.size();
    for (std::size_t i = 0; i < lap; ++i) {
        std::cout << list.popFront().value_or(0) << " ";
    }
    std::cout << "\n";
    list.display("After emptying");

    std::cout << "\nProgram finished successfully.\n";
    return 0;
}
