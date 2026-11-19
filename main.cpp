#include <iostream>
#include <string>
using namespace std;

int allocated = 0;
int freed = 0;
int passed = 0;
int total = 0;

struct EmployeeData {
    string fullName;
    string department;
    int value; 
};

struct DNode {
    EmployeeData data;
    DNode* prev;
    DNode* next;
};

struct DList {
    DNode* head;
    DNode* tail;
    int size;
};

struct CNode {
    EmployeeData data;
    CNode* next;
};

struct CList {
    CNode* head;
    CNode* tail;
    int size;
};

struct CDNode {
    EmployeeData data;
    CDNode* prev;
    CDNode* next;
};

struct CDList {
    CDNode* head;
    CDNode* tail;
    int size;
};

EmployeeData makeEmployee(int value) {
    EmployeeData emp;
    emp.value = value;
    emp.fullName = "Employee_" + to_string(value);
    emp.department = (value % 2 == 0) ? "IT" : "HR";
    return emp;
}

void initD(DList& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

DNode* makeD(int value) {
    DNode* p = new DNode;
    p->data = makeEmployee(value);
    p->prev = nullptr;
    p->next = nullptr;
    allocated++;
    return p;
}

void addD(DList& list, int value) {
    DNode* p = makeD(value);
    p->prev = list.tail;

    if (list.tail)
        list.tail->next = p;
    else
        list.head = p;

    list.tail = p;
    list.size++;
}

bool insertD(DList& list, int value, int pos) {
    if (pos < 0 || pos > list.size)
        return false;

    if (pos == list.size) {
        addD(list, value);
        return true;
    }

    DNode* current = list.head;

    for (int i = 0; i < pos; i++)
        current = current->next;

    DNode* p = makeD(value);
    p->next = current;
    p->prev = current->prev;

    if (current->prev)
        current->prev->next = p;
    else
        list.head = p;

    current->prev = p;
    list.size++;

    return true;
}

bool eraseD(DList& list, int pos) {
    if (pos < 0 || pos >= list.size)
        return false;

    DNode* p = list.head;

    for (int i = 0; i < pos; i++)
        p = p->next;

    if (p->prev)
        p->prev->next = p->next;
    else
        list.head = p->next;

    if (p->next)
        p->next->prev = p->prev;
    else
        list.tail = p->prev;

    delete p;
    freed++;
    list.size--;

    return true;
}

bool eraseValueD(DList& list, int value) {
    DNode* p = list.head;
    int pos = 0;

    while (p) {
        if (p->data.value == value)
            return eraseD(list, pos);
        p = p->next;
        pos++;
    }

    return false;
}

void showDForward(const DList& list) {
    DNode* p = list.head;

    while (p) {
        cout << p->data.value;

        if (p->next)
            cout << " <-> ";

        p = p->next;
    }

    if (!list.head)
        cout << "empty";
}

void showDBackward(const DList& list) {
    DNode* p = list.tail;

    while (p) {
        cout << p->data.value;

        if (p->prev)
            cout << " <-> ";

        p = p->prev;
    }

    if (!list.tail)
        cout << "empty";
}

bool checkD(const DList& list) {
    if (list.size == 0)
        return list.head == nullptr && list.tail == nullptr;

    if (!list.head || !list.tail || list.head->prev || list.tail->next)
        return false;

    int count = 0;
    DNode* p = list.head;
    DNode* previous = nullptr;

    while (p && count <= list.size) {
        if (p->prev != previous)
            return false;

        if (p->next && p->next->prev != p)
            return false;

        previous = p;
        p = p->next;
        count++;
    }

    if (p != nullptr || count != list.size || previous != list.tail)
        return false;

    count = 0;
    p = list.tail;
    DNode* next = nullptr;

    while (p && count <= list.size) {
        if (p->next != next)
            return false;

        next = p;
        p = p->prev;
        count++;
    }

    return p == nullptr && count == list.size && next == list.head;
}

void clearD(DList& list) {
    DNode* p = list.head;

    while (p) {
        DNode* next = p->next;
        delete p;
        freed++;
        p = next;
    }

    initD(list);
}

void initC(CList& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

void addC(CList& list, int value) {
    CNode* p = new CNode;
    p->data = makeEmployee(value);
    p->next = nullptr;
    allocated++;

    if (!list.head) {
        list.head = p;
        list.tail = p;
        p->next = p;
    }
    else {
        p->next = list.head;
        list.tail->next = p;
        list.tail = p;
    }

    list.size++;
}

bool insertC(CList& list, int value, int pos) {
    if (pos < 0 || pos > list.size)
        return false;

    if (pos == list.size) {
        addC(list, value);
        return true;
    }

    CNode* p = new CNode;
    p->data = makeEmployee(value);
    allocated++;

    if (pos == 0) {
        p->next = list.head;
        list.head = p;
        list.tail->next = list.head;
    }
    else {
        CNode* prev = list.head;

        for (int i = 0; i < pos - 1; i++)
            prev = prev->next;

        p->next = prev->next;
        prev->next = p;
    }

    list.size++;
    return true;
}

bool checkC(const CList& list) {
    if (list.size == 0)
        return !list.head && !list.tail;

    if (!list.head || !list.tail || list.tail->next != list.head)
        return false;

    CNode* p = list.head;

    for (int i = 0; i < list.size; i++) {
        if (!p)
            return false;

        p = p->next;

        if (i < list.size - 1 && p == list.head)
            return false;
    }

    return p == list.head;
}

void showCSteps(const CList& list, int steps) {
    if (!list.head) {
        cout << "empty";
        return;
    }

    CNode* p = list.head;

    for (int i = 0; i < steps; i++) {
        cout << p->data.value;

        if (i < steps - 1)
            cout << " -> ";

        p = p->next;
    }
}

void clearC(CList& list) {
    if (list.head) {
        CNode* p = list.head;

        for (int i = 0; i < list.size; i++) {
            CNode* next = p->next;
            delete p;
            freed++;
            p = next;
        }
    }

    initC(list);
}

void initCD(CDList& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

void addCD(CDList& list, int value) {
    CDNode* p = new CDNode;
    p->data = makeEmployee(value);
    allocated++;

    if (!list.head) {
        p->next = p;
        p->prev = p;
        list.head = p;
        list.tail = p;
    }
    else {
        p->prev = list.tail;
        p->next = list.head;
        list.tail->next = p;
        list.head->prev = p;
        list.tail = p;
    }

    list.size++;
}

bool insertCD(CDList& list, int value, int pos) {
    if (pos < 0 || pos > list.size)
        return false;

    if (pos == list.size) {
        addCD(list, value);
        return true;
    }

    CDNode* p = new CDNode;
    p->data = makeEmployee(value);
    allocated++;

    if (pos == 0) {
        if (!list.head) {
            p->next = p;
            p->prev = p;
            list.head = p;
            list.tail = p;
        }
        else {
            p->next = list.head;
            p->prev = list.tail;
            list.tail->next = p;
            list.head->prev = p;
            list.head = p;
        }
    }
    else {
        CDNode* current = list.head;
        for (int i = 0; i < pos; i++)
            current = current->next;

        p->next = current;
        p->prev = current->prev;
        current->prev->next = p;
        current->prev = p;
    }

    list.size++;
    return true;
}

bool checkCD(const CDList& list) {
    if (list.size == 0)
        return !list.head && !list.tail;

    if (!list.head || !list.tail)
        return false;

    if (list.head->prev != list.tail || list.tail->next != list.head)
        return false;

    CDNode* p = list.head;

    for (int i = 0; i < list.size; i++) {
        if (!p || !p->next || !p->prev)
            return false;

        if (p->next->prev != p || p->prev->next != p)
            return false;

        p = p->next;

        if (i < list.size - 1 && p == list.head)
            return false;
    }

    if (p != list.head)
        return false;

    p = list.tail;

    for (int i = 0; i < list.size; i++)
        p = p->prev;

    return p == list.tail;
}

void showCDForward(const CDList& list) {
    if (!list.head) {
        cout << "empty";
        return;
    }

    CDNode* p = list.head;

    for (int i = 0; i < list.size; i++) {
        cout << p->data.value;

        if (i < list.size - 1)
            cout << " <-> ";

        p = p->next;
    }
}

void showCDBackward(const CDList& list) {
    if (!list.tail) {
        cout << "empty";
        return;
    }

    CDNode* p = list.tail;

    for (int i = 0; i < list.size; i++) {
        cout << p->data.value;

        if (i < list.size - 1)
            cout << " <-> ";

        p = p->prev;
    }
}

void clearCD(CDList& list) {
    CDNode* p = list.head;

    for (int i = 0; i < list.size; i++) {
        CDNode* next = p->next;
        delete p;
        freed++;
        p = next;
    }

    initCD(list);
}

void testResult(const string& name, bool ok) {
    total++;

    cout << "Test " << total << " - " << name << ": "
        << (ok ? "PASSED" : "FAILED") << "\n";

    if (ok)
        passed++;
}

void runTests() {
    DList d;
    CList c;
    CDList cd;

    initD(d);
    testResult("empty doubly linked list", checkD(d));

    addD(d, 5);
    testResult("single node in doubly linked list",
        checkD(d) && d.head->prev == nullptr && d.head->next == nullptr);
    clearD(d);

    addD(d, 1);
    addD(d, 2);
    addD(d, 3);
    testResult("forward and backward links",
        checkD(d) && d.head->data.value == 1 && d.tail->data.value == 3);
    clearD(d);

    addD(d, 1);
    addD(d, 3);
    insertD(d, 2, 1);
    testResult("insert in the middle",
        checkD(d) && d.head->next->data.value == 2);
    clearD(d);

    addD(d, 1);
    addD(d, 2);
    addD(d, 3);
    eraseD(d, 1);
    testResult("delete from the middle",
        checkD(d) && d.size == 2 && d.head->next->data.value == 3);
    clearD(d);

    addD(d, 7);
    eraseD(d, 0);
    testResult("delete the only doubly linked node", checkD(d));

    addD(d, 1);
    addD(d, 2);
    insertD(d, 0, 0);
    testResult("insert at the beginning",
        checkD(d) && d.head->data.value == 0);
    clearD(d);

    addD(d, 1);
    addD(d, 2);
    eraseD(d, 1);
    testResult("delete the last node",
        checkD(d) && d.tail->data.value == 1);
    clearD(d);

    addD(d, 3);
    addD(d, 8);
    addD(d, 1);
    testResult("delete by key in doubly linked list",
        eraseValueD(d, 8) && checkD(d) && d.size == 2 && d.head->next->data.value == 1);
    clearD(d);

    initC(c);
    addC(c, 4);
    testResult("single node circular singly list",
        checkC(c) && c.head->next == c.head);
    clearC(c);

    addC(c, 1);
    addC(c, 2);
    addC(c, 3);
    testResult("five-step circular singly traversal",
        checkC(c) && c.head->next->next->next->next->data.value == 2);
    clearC(c);

    addC(c, 1);
    addC(c, 3);
    insertC(c, 2, 1);
    testResult("insert into circular singly list",
        checkC(c) && c.size == 3);
    clearC(c);

    addC(c, 1);
    addC(c, 2);
    addC(c, 3);
    testResult("circular singly list closes at head",
        checkC(c) && c.tail->next == c.head);
    clearC(c);

    initCD(cd);
    addCD(cd, 1);
    addCD(cd, 2);
    addCD(cd, 3);
    testResult("circular doubly forward links",
        checkCD(cd) && cd.head->next->data.value == 2);
    clearCD(cd);

    addCD(cd, 1);
    addCD(cd, 2);
    addCD(cd, 3);
    testResult("circular doubly backward links",
        checkCD(cd) && cd.tail->prev->data.value == 2);
    clearCD(cd);

    addCD(cd, 9);
    testResult("single node circular doubly list",
        checkCD(cd) && cd.head->next == cd.head && cd.head->prev == cd.head);
    clearCD(cd);

    testResult("clear circular doubly list", checkCD(cd) && cd.size == 0);

    addD(d, 1);
    addD(d, 2);
    addD(d, 3);

    DNode* saved = d.head->next->prev;
    d.head->next->prev = nullptr;

    bool detected = !checkD(d);
    d.head->next->prev = saved;

    testResult("detect damaged backward link", detected && checkD(d));
    clearD(d);

    addCD(cd, 1);
    addCD(cd, 2);

    CDNode* savedNext = cd.tail->next;
    cd.tail->next = nullptr;

    bool detectedCycle = !checkCD(cd);
    cd.tail->next = savedNext;

    testResult("detect broken circular link", detectedCycle && checkCD(cd));
    clearCD(cd);

    addC(c, 6);
    addC(c, 7);
    clearC(c);

    addC(c, 8);
    addC(c, 9);

    testResult("recreate circular singly list after clearing",
        checkC(c) && c.size == 2);
    clearC(c);
}

void runVariant() {
    int values[] = { 3, 8, 1, 6 };

    DList d;
    CList c;
    CDList cd;

    initD(d);
    initC(c);
    initCD(cd);

    for (int i = 0; i < 4; i++)
        addD(d, values[i]);

    insertD(d, 2, 0);

    cout << "\n=== VARIANT 5 ===\n";

    cout << "Doubly linked list after inserting 2 at beginning: ";
    showDForward(d);

    cout << "\nBackward traversal: ";
    showDBackward(d);

    cout << "\nDoubly list check: "
        << (checkD(d) ? "OK" : "ERROR") << "\n";

    for (int i = 0; i < 4; i++)
        addC(c, values[i]);

    insertC(c, 2, 0);

    cout << "Circular singly list, 8 steps: ";
    showCSteps(c, 8);

    cout << "\nCircular singly check: "
        << (checkC(c) ? "OK" : "ERROR") << "\n";

    for (int i = 0; i < 4; i++)
        addCD(cd, values[i]);

    insertCD(cd, 2, 0);

    cout << "Circular doubly forward: ";
    showCDForward(cd);

    cout << "\nCircular doubly backward: ";
    showCDBackward(cd);

    cout << "\nCircular doubly check: "
        << (checkCD(cd) ? "OK" : "ERROR") << "\n";

    clearD(d);
    clearC(c);
    clearCD(cd);

    cout << "After clearing all lists: "
        << ((d.size == 0 && c.size == 0 && cd.size == 0) ? "OK" : "ERROR")
        << "\n";
}

int main() {
    cout << "PRACTICAL WORK 13 - DOUBLY AND CIRCULAR LISTS\n";

    runTests();

    cout << "Test summary: " << passed << "/" << total << " passed\n";

    runVariant();

    cout << "\nAllocated nodes: " << allocated << "\n";
    cout << "Freed nodes: " << freed << "\n";
    cout << "Active nodes: " << allocated - freed << "\n";

    cout << ((allocated == freed)
        ? "Memory balance OK\n"
        : "MEMORY ERROR\n");

    return 0;
}
