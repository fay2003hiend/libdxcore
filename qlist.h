template<typename T>
class QList {

	class NODE {
	public:
		T data;
		NODE *prev;
		NODE *next;
	};

	NODE *list_head;
	int list_size;

	NODE *lastNode() {
		NODE *p = list_head;
		if (p == 0) {
			return 0;
		}

		while (p->next != 0) {
			p = p->next;
		}
		return p;
	}

	NODE *nodeAt(int index) {
		NODE *p = list_head;
		int i = 0;
		while (p != 0) {
			if (i++ == index) {
				return p;
			}
			p = p->next;
		}
		return 0;
	}

	void append(NODE *node) {
		if (list_head == 0) {
			list_head = node;
		} else {
			NODE *last = lastNode();
			last->next = node;
			node->prev = last;
		}
		list_size++;
	}

	void remove(NODE *p) {
		if (p == list_head) {
			list_head = p->next;
			if (p->next != 0) {
				p->next->prev = 0;
			}
		} else {
			if (p->prev != 0) {
				p->prev->next = p->next;
			}
			if (p->next != 0) {
				p->next->prev = p->prev;
			}
		}
		list_size--;
		delete p;
	}
public:
	QList() {
		list_head = 0;
		list_size = 0;
	}

	~QList() {
		clear();
	}

	inline bool isEmpty() {
		return list_size == 0;
	}

	void append(T data) {
		NODE *node = new NODE();
		node->data = data;
		node->prev = 0;
		node->next = 0;

		append(node);
	}

	void append(QList<T> list) {
		NODE *p = list.list_head;
		NODE *dest = lastNode();

		while (p != 0) {
			NODE *node = new NODE();
			node->data = p->data;
			node->prev = 0;
			node->next = 0;

			if (dest == 0) {
				list_head = node;
			} else {
				node->prev = dest;
				dest->next = node;
			}

			dest = node;
			list_size++;

			p = p->next;
		}
	}

	bool removeAt(int index) {
		NODE *p = nodeAt(index);

		if (p == 0) {
			return false;
		}

		remove(p);
		return true;
	}

	int indexOf(T data) {
		int i = -1;
		NODE *p = list_head;
		while (p != 0) {
			i++;
			if (p->data == data) {
				return i;
			}
			p = p->next;
		}
		return -1;
	}

	bool removeOne(T data) {
		bool found = false;
		NODE *p = list_head;
		while (p != 0) {
			if (p->data == data) {
				found = true;
				break;
			}
			p = p->next;
		}

		if (found) {
			remove(p);
		}
		return found;
	}

	T at(int index) {
		return nodeAt(index)->data;
	}

	T takeAt(int index) {
		NODE *p = nodeAt(index);
		T ret = p->data;
		remove(p);
		return ret;
	}

	inline T takeAtFirst() {
		return takeAt(0);
	}

	inline T operator [](int index) {
		return at(index);
	}

	void clear() {
		NODE *p = list_head;
		NODE *q = 0;

		while (p != 0) {
			q = p;
			p = p->next;
			delete q;
		}

		list_head = 0;
		list_size = 0;
	}

	int size() {
		return list_size;
	}
};
