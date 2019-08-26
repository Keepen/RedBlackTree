#include <iostream>
using namespace std;

//������Ľ��
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

//��װ������
template <class key, class value>
class RBIterator {
public:
	typedef RBTreeNode<key, value> Node;
	typedef Node* pNode;
	typedef pair<key, value> value_type;
	typedef RBIterator<key, value> Self;

	RBIterator(pNode node)
		:_node(node) {}

	//��������ִ��++��--��*��->����=��==
	//Ѱ����һ�����
	Self& operator++() {
		//�ж����Һ����Ƿ����
		//1.�Һ��Ӵ��ڣ����ҵ��Һ��ӵ�������
		if (_node->_right) {
			_node = _node->_right;
			while (_node->_left) {
				_node = _node->_left;
			}
			return *this;
		}
		//2.�Һ��Ӳ����ڣ����ϻ��ݣ�������丸�ڵ�����ӽ�㣬���ҵ��ýڵ�ĸ��ڵ㣬�����Ŀ��ڵ�
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
		//�ж��������Ƿ����
		//1.���Ӵ��ڣ�ֱ���ߵ������ӵ����ҽ��λ��
		if (_node->_left) {
			_node = _node->_left;
			while (_node->_right) {
				_node = _node->_right;
			}
			return *this;
		}
		//2.���Ӳ�����
		pNode parent = _node->_parent;
		//�ж����Ǹ��ڵ�����ӻ����Һ���
		//�����Һ��ӣ�ֱ��ָ�򸸽ڵ�
		while (parent->_left == _node) {
			_node = parent;
			parent = parent->_parent;
		}
		//�ж��Ƿ��˸��ڵ㴦
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


	//�ұߵ��ұ߸ߣ�����
	void RotateLeft(pNode node) {
		pNode subR = node->_right;
		pNode subRL = subR->_left;

		node->_right = subRL;
		subR->_left = node;

		//�ж�subRL�Ƿ���ڣ����ڣ���������˫��
		if (subRL) {
			subRL->_parent = node;
		}

		//pNode parent = node->_parent;
		//�жϸýڵ��Ƿ��Ǹ��ڵ�
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
		//�ýڵ��Ǹ��ڵ�
		else {
			_header->_parent = subR;
			subR->_parent = _header;
		}
		node->_parent = subR;
	}

	//��ߵ���߸ߣ�����
	void RotateRight(pNode node) {
		pNode subL = node->_left;
		pNode subLR = subL->_right;

		subL->_right = node;
		node->_left = subLR;

		//�ж�subLR�Ƿ���ڣ����ھ������丸�ڵ�
		if (subLR) {
			subLR->_parent = node;
		}

		//�жϸýڵ��Ƿ��Ǹ��ڵ�
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
		//�Ǹ��ڵ�
		else {
			_header->_parent = subL;
			subL->_parent = _header;
		}
		node->_parent = subL;
	}

	//����ָ��
	void Swap(pNode left, pNode right) {
		pNode cur = left;
		left = right;
		right = cur;
	}

	//�ҵ�������
	pNode LeftMost() {
		pNode cur = _header->_parent;
		while (cur->_left) {
			cur = cur->_left;
		}
		return cur;
	}

	//�ҵ����Һ���
	pNode RightMost() {
		pNode cur = _header->_parent;
		while (cur->_right) {
			cur = cur->_right;
		}
		return cur;
	}


	//���Ĳ���Ҳ�ǰ��ն�����������������
	bool Insert(const value_type& data) {
		//���ж����Ƿ�Ϊ��
		if (_header->_parent == nullptr) {
			pNode root = new Node(data);
			root->_color = BLACK;
			_header->_left = root;
			_header->_right = root;
			_header->_parent = root;
			root->_parent = _header;
			return true;
		}
		//���ǿգ����ն������������ҵ����ʵ�Ҷ��λ��
		pNode cur = _header->_parent;//�Ӹ��ڵ㴦��ʼ�������ҵ����ʵ�λ��
		pNode parent = nullptr;
		while (cur) {
			//���������ͬ�Ľ�㣬��ô�Ͳ��ٽ��в���
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
		//�ҵ��˺��ʵ�Ҷ�ӽ��,���������丸�ڵ�
		cur = new Node(data);
		if (parent->_data.first > data.first) {
			parent->_left = cur;
		}
		else {
			parent->_right = cur;
		}
		cur->_parent = parent;


		//�ж��½ڵ�ļ����Ƿ��ƻ��˺���������ʣ����е���
		while (parent && parent->_color == RED) {
			//�ж��Ƿ��������ĺ�ɫ�ڵ�
			pNode gparent = parent->_parent;
			//�ж�����ڵ����游����໹���Ҳ�
			if (gparent->_left && gparent->_left == parent) {
				pNode uncle = gparent->_right;
				//1.���������Ҳ�������Ǻ�ɫ
				if (uncle && uncle->_color == RED) {
					//�����ڵ�������ڵ�ĳɺ�ɫ���ٽ��游���ĳɺ�ɫ
					parent->_color = uncle->_color = BLACK;
					gparent->_color = RED;
					cur = gparent;
					parent = cur->_parent;
				}
				//2.����ڵ��Ǻ�ɫ�����߲����ڣ�
				//�����Ƿ���Ҫ˫��
				else if (parent->_right == cur) {
					//����parentΪ���������
					pNode last_parent = parent;
					pNode last_cur = cur;
					RotateLeft(parent);
					Swap(last_parent, last_cur);
				}
				//��parent���е���
				else {
					RotateRight(gparent);
				}
				//���е�ɫ��parent��ڣ�gparent���
				parent->_color = BLACK;
				gparent->_color = RED;
				break;
				//parent = cur->_parent;
				//gparent = parent->_parent;
			}
			//����ڵ����游�ڵ�����
			else {
				pNode uncle = gparent->_left;
				//�ж�����ڵ��Ƿ����
				//1.�������ڵ���ڲ���Ϊ��ɫ��ֻ��Ҫ�޸���ɫ����
				if (uncle && uncle->_color == RED) {
					parent->_color = uncle->_color = BLACK;
					gparent->_color = RED;
					cur = gparent;
					parent = cur->_parent;
				}
				//2.����ڵ㲻���ڣ����ߴ��ڵ����Ǻ�ɫ��
				//�ж��Ƿ���˫�������
				else if (parent->_left == cur) {
					//��Ҫ˫��������parentΪ���������
					pNode last_parent = parent;
					pNode last_cur = cur;
					RotateRight(parent);
					Swap(last_parent, last_cur);
				}
				//���е�������gparentΪ��
				else {
					RotateLeft(gparent);
				}
				//���е�ɫ
				parent->_color = BLACK;
				gparent->_color = RED;
				break;
			}
		}

		//ȷ�����ڵ����ɫʼ���Ǻ�ɫ
		_header->_parent->_color = BLACK;
		_header->_left = LeftMost();
		_header->_right = RightMost();
		return true;
	}

	bool isRBtree() {
		//�����������жϣ�
		//1.���ڵ�����Ǻ�ɫ��
		if (_header->_parent->_color == RED) {
			cout << "���ڵ��Ǻ�ɫ�ģ�����" << endl;
		}
		//�Ӹ��ڵ㵽ÿ��Ҷ�ӽ��·���ϵĺ�ɫ������Ŀ����ͬ�ģ�
		//�����������·�������ɫ������Ŀ
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
			//���ﰴ�պ��������������Ҳ������ڵ�
		//�ݹ�ʵ�֣��ݹ�ı߽硪���ߵ�Ҷ�ӽ��
		if (cur == nullptr) {
			//����ÿһ��·���ϵĺ�ɫ�����Ŀ�Ƿ���blackCount��ͬ
			if (blackCount == curBlackCount) {
				return true;
			}
			cout << "·���ϵĺ�ɫ�����Ŀ����ͬ������" << endl;
			return false;
		}

		if (cur->_color == BLACK) {
			++curBlackCount;
		}

		//�ж��Ƿ��������ĺ�ɫ�ڵ�
		pNode parent = cur->_parent;
		if (parent && parent->_color == RED && cur->_color == RED) {
			cout << "�������ĺ�ɫ�ڵ㣡����" << endl;
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
