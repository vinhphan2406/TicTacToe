#ifndef TEMP_TREE_H
#define TEMP_TREE_H

#include <vector>
#include <stdexcept>

template <class T>
struct Node {
    T* data;
    Node<T>* parent;
    std::vector<Node<T>*> child;
    Node(T* _data): data(_data), parent(NULL){}
    ~Node(){
        delete this->data;
    }
};

template <class T>
class Tree {
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

#ifndef MENU_COMPONENT_H
#define MENU_COMPONENT_H

#include <string>
#include <queue>

#include "Command.h"

class MenuComponent {
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

#endif

#ifndef MENU_COMMAND_H
#define MENU_COMMAND_H

#include <iostream>

// Interact with selected menu item (either execute a command or access another submenu)
class ChooseOptionCommand : public ICommand {
    private:
        Tree<MenuComponent>* menu;
        int option;
        int startNum;
    public:
        ChooseOptionCommand(Tree<MenuComponent>* _menu, int _option, int _startNum = 1): 
        menu(_menu), option(_option), startNum(_startNum){}
        // Interact with selected menu item (either execute a command or access another submenu)
        // OUTPUT:
        //  - 0: success
        //  - 1: execute command error
        //  - 2: invalid option value
        Result execute() override;
};

class ReturnPreviousMenu : public ICommand {
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

#endif


#ifndef DISPLAY_MENU_H
#define DISPLAY_MENU_H

#include "Display.h"
#include <iostream>

class DisplayMenu : public IDisplay {
    private:
        Tree<MenuComponent>* menu;
        bool printDesc;
        bool printIdx;
    public:
        DisplayMenu(Tree<MenuComponent>* _menu, bool _desc = false, bool _idx = true): 
        menu(_menu), printDesc(_desc), printIdx(_idx){}
        // Print current submenu
        // OUTPUT:
        //  - 0: success
        int print() override;
};

#endif