#ifndef XML_PARSING_BT_H
#define XML_PARSING_BT_H

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/blackboard/blackboard_local.h"

namespace BT
{

/**
 * @brief Struct used to store a tree.
 * If this object goes out of scope, the tree is destroyed.
 *
 * To tick the tree, simply call:
 *
 *    NodeStatus status = my_tree.root_node->executeTick();
 */
struct Tree
{
    TreeNode* root_node;
    std::vector<TreeNode::Ptr> nodes;
    std::vector<Blackboard::Ptr> blackboard_stack;

    Tree() : root_node(nullptr)
    { }

    ~Tree()
    {
        if (root_node) {
            haltAllActions(root_node);
        }
    }

    Blackboard::Ptr rootBlackboard()
    {
        if( blackboard_stack.size() > 0)
        {
            return blackboard_stack.front();
        }
        return {};
    }
};

/**
 * @brief The XMLParser is a class used to read the model
 * of a BehaviorTree from file or text and instantiate the
 * corresponding tree using the BehaviorTreeFactory.
 */
class XMLParser
{
  public:
    XMLParser(const BehaviorTreeFactory& factory);

    ~XMLParser();

    XMLParser(const XMLParser& other) = delete;
    XMLParser& operator=(const XMLParser& other) = delete;

    void loadFromFile(const std::string& filename);

    void loadFromText(const std::string& xml_text);

    Tree instantiateTree(const Blackboard::Ptr &root_blackboard);

  private:

    struct Pimpl;
    Pimpl* _p;

};



/** Helper function to do the most common steps, all at once:
* 1) Create an instance of XMLParse and call loadFromText.
* 2) Instantiate the entire tree.
*
*/
Tree buildTreeFromText(const BehaviorTreeFactory& factory,
                       const std::string& text,
                       Blackboard::Ptr blackboard = Blackboard::create<BlackboardLocal>());

/** Helper function to do the most common steps all at once:
* 1) Create an instance of XMLParse and call loadFromFile.
* 2) Instantiate the entire tree.
*
*/
Tree buildTreeFromFile(const BehaviorTreeFactory& factory,
                       const std::string& filename,
                       Blackboard::Ptr blackboard = Blackboard::create<BlackboardLocal>());

std::string writeXML(const BehaviorTreeFactory& factory,
                     const TreeNode* root_node,
                     bool compact_representation = false);
}

#endif   // XML_PARSING_BT_H
