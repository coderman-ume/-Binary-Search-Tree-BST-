#include <iostream>
#include <limits>
#include <memory>

struct Node {
    int data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    explicit Node(int value) : data(value) {}
};

using NodePtr = std::unique_ptr<Node>;

enum class InsertResult { Inserted, Duplicate };

InsertResult insertValue(NodePtr &node, int value) {
    if (!node) {
        node = std::make_unique<Node>(value);
        return InsertResult::Inserted;
    }
    if (value == node->data) {
        return InsertResult::Duplicate;
    }
    if (value < node->data) {
        return insertValue(node->left, value);
    }
    return insertValue(node->right, value);
}

bool searchValue(const Node *node, int value) {
    if (!node) {
        return false;
    }
    if (value == node->data) {
        return true;
    }
    if (value < node->data) {
        return searchValue(node->left.get(), value);
    }
    return searchValue(node->right.get(), value);
}

bool deleteValue(NodePtr &node, int value) {
    if (!node) {
        return false;
    }
    if (value < node->data) {
        return deleteValue(node->left, value);
    }
    if (value > node->data) {
        return deleteValue(node->right, value);
    }
    if (!node->left) {
        node = std::move(node->right);
        return true;
    }
    if (!node->right) {
        node = std::move(node->left);
        return true;
    }
    const Node *successor = node->right.get();
    while (successor->left) {
        successor = successor->left.get();
    }
    const int successorValue = successor->data;
    node->data = successorValue;
    deleteValue(node->right, successorValue);
    return true;
}

void printInorder(const Node *node, bool &isFirst) {
    if (!node) {
        return;
    }
    printInorder(node->left.get(), isFirst);
    if (!isFirst) {
        std::cout << ' ';
    }
    std::cout << node->data;
    isFirst = false;
    printInorder(node->right.get(), isFirst);
}

void displayInorder(const NodePtr &root) {
    if (!root) {
        std::cout << "The BST is empty.\n";
        return;
    }
    bool isFirst = true;
    std::cout << "Inorder traversal: ";
    printInorder(root.get(), isFirst);
    std::cout << '\n';
}

void printMenu() {
    std::cout << "1. Insert a value\n"
              << "2. Search for a value\n"
              << "3. Delete a value\n"
              << "4. Display inorder traversal\n"
              << "5. Exit\n";
}

bool readInt(int &value) {
    if (std::cin >> value) {
        return true;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

void handleInsert(NodePtr &root) {
    std::cout << "Enter value to insert: ";
    int value = 0;
    if (!readInt(value)) {
        std::cout << "Invalid input. Enter an integer.\n";
        return;
    }
    if (insertValue(root, value) == InsertResult::Duplicate) {
        std::cout << "Value " << value << " already exists. Duplicate not inserted.\n";
        return;
    }
    std::cout << "Inserted " << value << ".\n";
}

void handleSearch(const NodePtr &root) {
    if (!root) {
        std::cout << "The BST is empty.\n";
        return;
    }
    std::cout << "Enter value to search: ";
    int value = 0;
    if (!readInt(value)) {
        std::cout << "Invalid input. Enter an integer.\n";
        return;
    }
    if (searchValue(root.get(), value)) {
        std::cout << "Value " << value << " found in the BST.\n";
    } else {
        std::cout << "Value " << value << " not found in the BST.\n";
    }
}

void handleDelete(NodePtr &root) {
    if (!root) {
        std::cout << "The BST is empty.\n";
        return;
    }
    std::cout << "Enter value to delete: ";
    int value = 0;
    if (!readInt(value)) {
        std::cout << "Invalid input. Enter an integer.\n";
        return;
    }
    if (!deleteValue(root, value)) {
        std::cout << "Value " << value << " not found in the BST.\n";
        return;
    }
    std::cout << "Deleted " << value << ".\n";
    displayInorder(root);
}

int main() {
    NodePtr root;
    bool isRunning = true;
    while (isRunning) {
        printMenu();
        std::cout << "Enter choice: ";
        int choice = 0;
        if (!readInt(choice)) {
            std::cout << "Invalid input. Enter a number from 1 to 5.\n";
            continue;
        }
        switch (choice) {
            case 1:
                handleInsert(root);
                break;
            case 2:
                handleSearch(root);
                break;
            case 3:
                handleDelete(root);
                break;
            case 4:
                displayInorder(root);
                break;
            case 5:
                isRunning = false;
                break;
            default:
                std::cout << "Invalid choice. Enter a number from 1 to 5.\n";
                break;
        }
    }
    root.reset();
    std::cout << "All nodes freed. Program ended.\n";
    return 0;
}
