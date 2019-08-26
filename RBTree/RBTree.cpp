#include <iostream>
using namespace std;

//红黑树的结点
enum Color { RED, BLACK };

template <class key, class value>
struct RBTreeNode {
	typedef RBTreeNode<key, value> Node;
	typedef pair<key, value> value_type;
	value_type _data;
	Node* _left;
	Node* _right;
	Node* _parent;
	Color _color;
	RBTreeNode(const value_type& data = value_type())
		:_data(data),
		_left(nullptr),
		_right(nullptr),
		_parent(nullptr),
		_color(RED) {}
};

//封装迭代器
template <class key, class value>
class RBIterator {
public:
	typedef RBTreeNode<key, value> Node;
	typedef Node* pNode;
	typedef pair<key, value> value_type;
	typedef RBIterator<key, value> Self;

	RBIterator(pNode node)
		:_node(node) {}

	//迭代器，执行++，--，*，->，！=，==
	//寻找下一个结点
	Self& operator++() {
		//判断其右孩子是否存在
		//1.右孩子存在，就找到右孩子的最左结点
		if (_node->_right) {
			_node = _node->_right;
			while (_node->_left) {
				_node = _node->_left;
			}
			return *this;
		}
		//2.右孩子不存在，向上回溯，如果是其父节点的左孩子结点，就找到该节点的父节点，其就是目标节点
		pNode parent = _node->_parent;
		while (parent->_right == _node) {
			_node = parent;
			parent = parent->_parent;
		}
		
		if (_node->_right!=parent) {
			_node = _node->_parent;
		}
		return *this;
	}

	Self& operator--() {
		//判断其左孩子是否存在
		//1.左孩子存在，直接走到其左孩子的最右结点位置
		if (_node->_left) {
			_node = _node->_left;
			while (_node->_right) {
				_node = _node->_right;
			}
			return *this;
		}
		//2.左孩子不存在
		pNode parent = _node->_parent;
		//判断它是父节点的左孩子还是右孩子
		//若是右孩子，直接指向父节点
		while (parent->_left == _node) {
			_node = parent;
			parent = parent->_parent;
		}
		//判断是否到了根节点处
		if (_node->_left != parent) {
			_node = parent;
		}
		return *this;
	}


	value_type& operator*() {
		return _node->_data;
	}

	value_type* operator->() {
		return &_node->_data;
	}

	bool operator!=(const Self& it) {
		return _node != it._node;
	}

	bool operator==(const Self& it) {
		return _node == it._node;
	}


private:
	pNode _node;
};


template <class key, class value>
class RBTree {
public:
	typedef RBTreeNode<key, value> Node;
	typedef Node* pNode;
	typedef pair<key, value> value_type;
	typedef RBIterator<key, value> iterator;
	RBTree(const value_type& data = value_type()) {
		_header = new Node(data);
		_header->_left = _header;
		_header->_right = _header;
		_header->_parent = nullptr;
		_header->_color = BLACK;
	}

	iterator begin() {
		return iterator(_header->_left);
	}

	iterator end() {
		return iterator(_header);
	}


	//右边的右边高，左旋
	void RotateLeft(pNode node) {
		pNode subR = node->_right;
		pNode subRL = subR->_left;

		node->_right = subRL;
		subR->_left = node;

		//判断subRL是否存在，存在，则链接其双亲
		if (subRL) {
			subRL->_parent = node;
		}

		//pNode parent = node->_parent;
		//判断该节点是否是根节点
		if (node != nullptr) {
			pNode parent = node->_parent;
			if (parent->_left == parent) {
				parent->_left = subR;
			}
			else {
				parent->_right = subR;
			}
			subR->_parent = parent;
		}
		//该节点是根节点
		else {
			_header->_parent = subR;
			subR->_parent = _header;
		}
		node->_parent = subR;
	}

	//左边的左边高，右旋
	void RotateRight(pNode node) {
		pNode subL = node->_left;
		pNode subLR = subL->_right;

		subL->_right = node;
		node->_left = subLR;

		//判断subLR是否存在，存在就链接其父节点
		if (subLR) {
			subLR->_parent = node;
		}

		//判断该节点是否是根节点
		if (node != nullptr) {
			pNode parent = node->_parent;
			if (parent->_left == node) {
				parent->_left = subL;
			}
			else {
				parent->_right = subL;
			}
			subL->_parent = parent;
		}
		//是根节点
		else {
			_header->_parent = subL;
			subL->_parent = _header;
		}
		node->_parent = subL;
	}

	//交换指针
	void Swap(pNode left, pNode right) {
		pNode cur = left;
		left = right;
		right = cur;
	}

	//找到最左孩子
	pNode LeftMost() {
		pNode cur = _header->_parent;
		while (cur->_left) {
			cur = cur->_left;
		}
		return cur;
	}

	//找到最右孩子
	pNode RightMost() {
		pNode cur = _header->_parent;
		while (cur->_right) {
			cur = cur->_right;
		}
		return cur;
	}


	//结点的插入也是按照二叉搜索树的性质来
	bool Insert(const value_type& data) {
		//先判断树是否为空
		if (_header->_parent == nullptr) {
			pNode root = new Node(data);
			root->_color = BLACK;
			_header->_left = root;
			_header->_right = root;
			_header->_parent = root;
			root->_parent = _header;
			return true;
		}
		//树非空，按照二叉树的性质找到合适的叶子位置
		pNode cur = _header->_parent;//从根节点处开始遍历，找到合适的位置
		pNode parent = nullptr;
		while (cur) {
			//如果遇到相同的结点，那么就不再进行插入
			if (cur->_data.first == data.first) {
				return false;
			}
			parent = cur;
			if (cur->_data.first > data.first) {
				cur = cur->_left;
			}
			else {
				cur = cur->_right;
			}
		}
		//找到了合适的叶子结点,重新链接其父节点
		cur = new Node(data);
		if (parent->_data.first > data.first) {
			parent->_left = cur;
		}
		else {
			parent->_right = cur;
		}
		cur->_parent = parent;


		//判断新节点的加入是否破坏了红黑树的性质，进行调整
		while (parent && parent->_color == RED) {
			//判断是否有连续的红色节点
			pNode gparent = parent->_parent;
			//判断叔叔节点在祖父的左侧还是右侧
			if (gparent->_left && gparent->_left == parent) {
				pNode uncle = gparent->_right;
				//1.叔叔结点在右侧存在且是红色
				if (uncle && uncle->_color == RED) {
					//将父节点与叔叔节点改成黑色，再将祖父结点改成红色
					parent->_color = uncle->_color = BLACK;
					gparent->_color = RED;
					cur = gparent;
					parent = cur->_parent;
				}
				//2.叔叔节点是黑色，或者不存在，
				//考虑是否需要双旋
				else if (parent->_right == cur) {
					//先以parent为轴进行左旋
					pNode last_parent = parent;
					pNode last_cur = cur;
					RotateLeft(parent);
					Swap(last_parent, last_cur);
				}
				//以parent进行单旋
				else {
					RotateRight(gparent);
				}
				//进行调色，parent变黑，gparent变红
				parent->_color = BLACK;
				gparent->_color = RED;
				break;
				//parent = cur->_parent;
				//gparent = parent->_parent;
			}
			//叔叔节点在祖父节点的左边
			else {
				pNode uncle = gparent->_left;
				//判断叔叔节点是否存在
				//1.如果叔叔节点存在并且为红色，只需要修改颜色即可
				if (uncle && uncle->_color == RED) {
					parent->_color = uncle->_color = BLACK;
					gparent->_color = RED;
					cur = gparent;
					parent = cur->_parent;
				}
				//2.叔叔节点不存在，或者存在但是是黑色的
				//判断是否有双旋的情况
				else if (parent->_left == cur) {
					//需要双旋，先以parent为轴进行右旋
					pNode last_parent = parent;
					pNode last_cur = cur;
					RotateRight(parent);
					Swap(last_parent, last_cur);
				}
				//进行单旋，以gparent为轴
				else {
					RotateLeft(gparent);
				}
				//进行调色
				parent->_color = BLACK;
				gparent->_color = RED;
				break;
			}
		}

		//确保根节点的颜色始终是黑色
		_header->_parent->_color = BLACK;
		_header->_left = LeftMost();
		_header->_right = RightMost();
		return true;
	}

	bool isRBtree() {
		//按照性质来判断：
		//1.根节点必须是黑色的
		if (_header->_parent->_color == RED) {
			cout << "根节点是红色的！！！" << endl;
		}
		//从根节点到每个叶子结点路径上的黑色结点的数目是相同的；
		//先沿着最左边路径计算黑色结点的数目
		int blackCount = 0;
		pNode cur = _header->_parent;
		while (cur) {
			if (cur->_color == BLACK) {
				++blackCount;
			}
			cur = cur->_left;
		}
		
		int curBlackCount = 0;
		cur = _header->_parent;
		return _isRBtree(cur, blackCount, curBlackCount);
	}

	bool _isRBtree(pNode cur,int blackCount,int curBlackCount) {
			//这里按照后序遍历，否则会找不到根节点
		//递归实现，递归的边界——走到叶子结点
		if (cur == nullptr) {
			//计算每一条路径上的黑色结点数目是否与blackCount相同
			if (blackCount == curBlackCount) {
				return true;
			}
			cout << "路径上的黑色结点数目不相同！！！" << endl;
			return false;
		}

		if (cur->_color == BLACK) {
			++curBlackCount;
		}

		//判断是否有连续的红色节点
		pNode parent = cur->_parent;
		if (parent && parent->_color == RED && cur->_color == RED) {
			cout << "有连续的红色节点！！！" << endl;
			return false;
		}

		return _isRBtree(cur->_left, blackCount, curBlackCount)
			&& _isRBtree(cur->_right, blackCount, curBlackCount);
	}



private:
	pNode _header;
};


void test() {
	RBTree<int, int> rbt;
	rbt.Insert(make_pair(2, 2));
	rbt.Insert(make_pair(3, 3));
	rbt.Insert(make_pair(1, 1));
	rbt.Insert(make_pair(4, 4));
	rbt.Insert(make_pair(5, 5));

	for (auto& e : rbt) {
		cout << e.first << "-->" << e.second << endl;
	}
	cout << rbt.isRBtree() << endl;

}

int main() {
	test();
	system("pause");
	return 0;
}
