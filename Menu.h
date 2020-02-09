// #ifndef TEMPLATE_TREE
// #define TEMPLATE_TREE

// #include <vector>

// template<class T>
// struct Node{
//     T* data;
//     Node<T> *parent;
//     std::vector<Node<T>*> child;
//     Node(): data(NULL), parent(NULL){}
//     Node(T* _data): data(_data), parent(NULL){}
// };

// template<class T>
// class Tree{
//     private:
//         Node<T> *pHead;
//         Node<T> *currentNode;
//     protected:
//         Node<T> *insertNode(Node<T>* _node, T* _data);
//         Node<T> *removeNode(Node<T>* _node);
//     public:
//         Tree(): pHead(NULL), currentNode(NULL){}
//         ~Tree(){
//             this->pHead = removeNode(this->pHead);
//         }
//         bool insertNode(T* _data){
//             if (currentNode == NULL) return false;
//             this->currentNode = insertNode(this->currentNode, _data);
//         }
//         bool removeNode(){
//             if (this->currentNode == NULL) return false;
//             Node<T>* tempNode = this->currentNode;
//             this->currentNode = this->currentNode->parent;
//             for (typename std::vector<Node<T>*>::iterator it = this->currentNode->child.begin(); it != this->currentNode->child.end(); it++){
//                 if (*it == tempNode){
//                     this->currentNode->child.erase(it);
//                     break;
//                 }
//             }
//             this->currentNode = removeNode(this->currentNode);
//             return true;
//         }
//         bool moveUp(){
//             if (this->currentNode == NULL) return false;
//             this->currentNode = this->currentNode->parent;
//             return true;
//         }
//         bool moveDown(int _idx){
//             if (this->currentNode == NULL){
//                 this->currentNode = this->pHead;
//                 return this->pHead != NULL;
//             }
//             if (this->currentNode->child.empty() == true) return false;
//             this->currentNode = this->currentNode->child[_idx];
//             return true;
//         }
//         T* getData(){
//             return this->currentNode->data;
//         }
// };

// template<class T>
// // Insert node to tree from starting node as child
// // INPUT:
// // OUTPUT:
// //  modified _node
// Node<T>* Tree<T>::insertNode(Node<T>* _root, T* _data){
//     if (_root == NULL) _root = new Node<T>(_data);
//     if (_data != NULL) _root->child.push_back(new Node<T>(_data));
//     return _root;
// }


// template<class T>
// // Recursively delete nodes from tree from starting node
// // INPUT:
// //  _node: starting node
// // OUTPUT:
// //  modified _node
// Node<T>* Tree<T>::removeNode(Node<T>* _root){
//     if (_root == NULL) return _root;
//     for (typename std::vector<Node<T>*>::iterator it = _root->child.begin(); it != _root->child.end(); it++){
//         *it = removeNode(*it);
//         delete *it;
//         *it = NULL;
//     }
//     _root = removeNode(_root);
//     delete _root;
//     _root = NULL;
//     return _root;
// }
// #endif

#ifndef TEMP_TREE_H
#define TEMP_TREE_H

#include <vector>
#include <stdexcept>

template <class T>
struct Node{
    T* data;
    Node<T>* parent;
    std::vector<Node<T>*> child;
    Node(T* _data): data(_data), parent(NULL){}
    ~Node(){
        delete this->data;
    }
};

template <class T>
class Tree{
    private:
        Node<T>* pHead;
        Node<T>* currentNode;
    public:
        Tree(): currentNode(NULL){}
        virtual ~Tree(){
            clear();
        }
        bool clear(){
            this->pHead = removeNode(this->pHead);
            this->currentNode = NULL;
        }
        bool moveUp(){
            if (this->currentNode == NULL) return false;
            if (this->currentNode->parent == NULL) return false;
            this->currentNode = this->currentNode->parent;
            return true;
        }
        bool moveDown(int _idx){
            if (this->currentNode == NULL) return false;
            if (this->currentNode->child.empty()) return false;
            if (_idx < 0 || _idx >= this->currentNode->child.size()) 
                throw std::out_of_range("Index: Out of range!");
            this->currentNode = this->currentNode->child[_idx];
            return true;
        }
        Node<T>* insertNode(Node<T>* _root, T* _data){
            if (_root == NULL) return NULL;
            Node<T>* tempNode = new Node<T>(_data);
            tempNode->parent = _root;
            this->currentNode->child.push_back(tempNode);
            return _root;
        }
        bool insertNode(T* _data){
            if (!(this->currentNode = insertNode(this->currentNode, _data))){
                this->currentNode = new Node<T>(_data);
                this->pHead = this->currentNode;
            }
            return true;
        }
        Node<T>* removeNode(Node<T>* _root){
            if (_root != NULL){
                for (typename std::vector<Node<T>*>::iterator it = _root->child.begin(); it != _root->child.end(); it++){
                    *it = removeNode(*it);
                }
                delete _root;
            }
            return NULL;
        }
        bool removeNode(){
            if (this->currentNode == NULL) return false;
            if (this->currentNode->parent == NULL){
                this->currentNode = removeNode(this->currentNode);
                this->pHead = NULL;
                return true;
            }
            for (typename std::vector<Node<T>*>::iterator it = this->currentNode->parent->child.begin(); 
            it != this->currentNode->parent->child.end(); it++){
                if (this->currentNode == *it){
                    this->currentNode = this->currentNode->parent;
                    *it = removeNode(*it);
                    it = this->currentNode->child.erase(it);
                    break;
                }
            }
            return true;
        }
        T*& getCurrentData(){
            if (this->currentNode == NULL)
                throw std::bad_alloc();
            return this->currentNode->data;
        }
        bool hasChild(){
            if (this->currentNode == NULL) return false;
            return this->currentNode->child.empty() == false;
        }
        bool isEmpty(){
            return (this->pHead == NULL);
        }
        T*& getChildData(int _idx){
            if (hasChild() == false)
                throw std::invalid_argument("Node: Sub-element not exist!");
            if (_idx < 0 || _idx >= this->currentNode->child.size()) 
                throw std::out_of_range("Index: Out of range!");
            return this->currentNode->child[_idx]->data;
        }
        bool traverseDFS(Node<T>* _root, void (*func)(T*, void*), void* pParam){
            if (_root == NULL) return true;
            func(_root->data, pParam);
            bool result = true;
            for (typename std::vector<Node<T>*>::iterator it = _root->child.begin(); it != _root->child.end(); it++){
                result = result && traverse(*it, func, pParam);
            }
            return result;
        }
        bool traverse(void (*func)(T*, void*), void* pParam){
            return traverseDFS(this->currentNode, func, pParam);
        }
        int getChildSize(){
            if (this->currentNode == NULL) return 0;
            return this->currentNode->child.size();
        }
        bool returnHead(){
            if (this->pHead == this->currentNode) return true;
            this->currentNode = this->pHead;
            return true;
        } 
};

#endif

#ifndef MENU_H
#define MENU_H

#include <string>
#include <queue>

#include "Command.h"

class MenuComponent{
    protected:
        std::string title;
        std::string desc;
        std::vector<ICommand*> commands;
        bool isHidden;
    public:
        MenuComponent(std::string _title = "", std::string _desc = "", bool _isHidden = false): title(_title), desc(_desc), isHidden(_isHidden){}
        ~MenuComponent(){
            for (std::vector<ICommand*>::iterator it = this->commands.begin(); it != this->commands.end(); it++){
                delete *it;
            }
        }
        // Get title of submenu
        std::string getTitle(){
            return this->title;
        }
        // Get description of submenu
        std::string getDescription(){
            return this->desc;
        }
        // Set the option hidden from submenu
        bool setHidden(bool _isHidden = true){
            this->isHidden = _isHidden;
            return true;
        }
        // Get state of "isHidden" property option in submenu
        // OUTPUT:
        //  - true: option is hidden
        //  - false: option is not hidden
        bool getHiddenState(){
            return this->isHidden;
        }
        ICommand::Result doAction();
        bool loadNextAction(ICommand* _command);
        bool removeLastAction();

};
// Interact with selected menu item (either execute a command or access another submenu)
class ChooseOptionCommand : public ICommand{
    private:
        Tree<MenuComponent>* menu;
        int option;
    public:
        ChooseOptionCommand(Tree<MenuComponent>* _menu, int _option) : menu(_menu), option(_option){}
        // Interact with selected menu item (either execute a command or access another submenu)
        // OUTPUT:
        //  - 0: success
        //  - 1: execute command error
        //  - 2: invalid option value
        Result execute() override;
};

class ReturnPreviousMenu : public ICommand{
    private:
        Tree<MenuComponent>* menu;
    public:
        ReturnPreviousMenu(Tree<MenuComponent>* _menu): menu(_menu){}
        // Return to previous upper menu
        // OUTPUT:
        //  - 0: success
        //  - 1: no upper menu available
        Result execute() override;
};

// class MenuItem : public MenuComponent{
//     public:
//         MenuItem(std::string _title, std::string _desc){
//             this->title = _title;
//             this->desc = _desc;
//             this->commands.push_back(new );
//         }
// };

// class Menu : public MenuComponent{
//     public:
//         Menu(std::string _title, std::string _desc){
//             this->title = _title;
//             this->desc = _desc;
//             this->commands.push_back(new );
//         }
// };
#endif


