#include <iostream>
#include <unordered_map>

#define BUFFER 5

using namespace std;

typedef struct Node {
    int key;
    long long int value;
    Node *prev;
    Node *next;
} Node;

long long int calc_result(int, unordered_map<int, Node *> &, int &, Node *&, Node *&);

int main() {
    int buffer_count = 0;
    long long int result;
    unordered_map<int, Node *> map;
    Node *head = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < 10; i++) {
        result = calc_result(i, map, buffer_count, head, tail);
        cout << result << endl;
    }

    result = calc_result(5, map, buffer_count, head, tail);
    cout << result
         << " -> 5! now moved to the tail since it was just used. It's preceded by 6, 7, 8, 9 factorial"
         << endl;

    result = calc_result(20, map, buffer_count, head, tail);
    cout << result << " -> 6! should now be removed from the buffer" << endl;

    result = calc_result(6, map, buffer_count, head, tail);
    cout << result << " -> confirms 6! needed to be recalculated" << endl;

    result = calc_result(5, map, buffer_count, head, tail);
    cout << result << " -> 5! still buffered since it is the 3rd most recently used" << endl;

    return 0;
}

long long int calc_result(int input, unordered_map<int, Node *> &map, int &buffer_count, Node *&head, Node *&tail) {
    if (map.contains(input)) {
        Node *node = map.at(input);
        if (node != tail) {
            if (node == head) {
                head = node->next;
            }
            node->next->prev = node->prev;
            node->prev = tail;
            node->next = nullptr;
            tail->next = node;
            tail = node;
        }
        cout << "Got " << input << "! from the buffer: ";
        return map.at(input)->value;
    } else {
        long long int result = 1;
        for (int i = 2; i <= input; i++) {
            result *= i;
        }

        Node *node = new Node{input, result, nullptr, nullptr};

        if (buffer_count == 0) {
            head = node;
            tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }

        // remove the least recently used node from the buffer if it's full
        if (buffer_count >= BUFFER) {
            map.erase(head->key);
            Node *next = head->next;
            delete head;
            head = next;
            head->prev = nullptr;
            buffer_count--;
        }

        map[input] = node;
        buffer_count++;
        cout << "calculated " << input << "! = ";
        return result;
    }
}
