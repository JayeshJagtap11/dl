#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <omp.h>
using namespace std;

class node {
public:
	node *left, *right;
	int data;
};

class Breadthfs {
public:
	node* insert(node* root, int data);
	void bfs(node* head);
};

node* Breadthfs::insert(node* root, int data) {
	if (!root) {
		root = new node;
		root->left = NULL;
		root->right = NULL;
		root->data = data;
		return root;
	}

	queue<node*> q;
	q.push(root);

	while (!q.empty()) {
		node* temp = q.front();
		q.pop();

		if (temp->left == NULL) {
			temp->left = new node;
			temp->left->left = NULL;
			temp->left->right = NULL;
			temp->left->data = data;
			return root;
		}
		else {
			q.push(temp->left);
		}

		if (temp->right == NULL) {
			temp->right = new node;
			temp->right->left = NULL;
			temp->right->right = NULL;
			temp->right->data = data;
			return root;
		}
		else {
			q.push(temp->right);
		}
	}
	return root;
}

void Breadthfs::bfs(node* head) {
	if (!head) return;

	queue<node*> q;
	q.push(head);

	while (!q.empty()) {
		int qSize = q.size();
		vector<node*> currentLevel;

		for (int i = 0; i < qSize; i++) {
			currentLevel.push_back(q.front());
			q.pop();
		}

		// #pragma omp parallel for
		for (int i = 0; i < currentLevel.size(); i++) {
			cout << "\t" << currentLevel[i]->data;
		}

		// After printing, collect children (outside parallel)
		for (int i = 0; i < currentLevel.size(); i++) {
			if (currentLevel[i]->left) q.push(currentLevel[i]->left);
			if (currentLevel[i]->right) q.push(currentLevel[i]->right);
		}
	}
}

int main() {
	Breadthfs tree;
	node* root = NULL;
	int data;
	char ans;

	do {
		cout << "\nEnter data => ";
		cin >> data;

		root = tree.insert(root, data);

		cout << "Do you want to insert one more node? (y/n) => ";
		cin >> ans;

	} while (ans == 'y' || ans == 'Y');

	double start = omp_get_wtime();
	tree.bfs(root);
	double end = omp_get_wtime();

	cout << "\nExecution Time: " << (end - start) << " seconds\n";

	return 0;
}
