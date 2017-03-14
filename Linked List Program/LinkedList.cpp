#include <stdio.h>

struct Node
{
	int val; // Value within the node
	Node* next; // Pointer to the next node
};

Node * sortList(Node * currentHead) // Sorts the given sequence
{
	if (currentHead == 0) // If empty list. 0 == NULL
	{
		return currentHead;
	}
	else if (currentHead->next == 0) // If list is only one item
	{
		return currentHead;
	}

	// Setting up the linked list
	Node* oldHead = currentHead; // Keeps track of the last headnode
	Node* newHead = currentHead; // Will be set to the new headnode, the node with the largest unsorted value
	Node* curNode = newHead; // Node currently being examined
	Node* nextNode = curNode->next;
	Node* prevNode = curNode; // Keeps track of the node just before the node with the largest unsorted value, used to reorder list properly
	int curLargest = newHead->val; // Keeps track of largest value in a cycle
	int smallest = newHead->val; // Keeps track of the smallest value overall
	int nodesSorted = 0;

	while (nextNode != 0) // First pass of list goes through while there are still nodes in the list it hasn't examined
	{
		if (nextNode->val < smallest) // Finding the smallest value
		{
			smallest = nextNode->val;
		}

		if ((0 - nextNode->val) <= (0 - curLargest)) // Finds the largest node. Will sort by finding the largest unsorted node and moving it to the front, eventually getting the list sorted by smallest to largest
		{
			prevNode = curNode; // Keep track of the node before the largest value node
			newHead = nextNode; // Set the future new head of the list
			curLargest = newHead->val; // Set the new largest unsorted value
		}

		// Iterate through the list
		curNode = nextNode;
		nextNode = curNode->next;
	}

	if (newHead->next == 0) // If the largest value was the last node
	{
		prevNode->next = 0; // Make the next pointer for the node before it NULL
	}
	else
	{
		prevNode->next = newHead->next; // Make the next pointer for the node before it point to the largest node's next node
	}
	newHead->next = oldHead; // Set the new headnode at the front and point its next pointer toward the previous headnode
	oldHead = newHead; // Set the head to the new headnode
	curNode = newHead; // Reset to the beginning of the list
	nextNode = curNode->next; // Reset position
	prevNode = curNode; // Reset position
	nodesSorted++;

	while (oldHead->val != smallest) // While the value at the beginning of the list is not the smallest number, which would indicate the list is sorted
	{
		for (int i = 0; i < nodesSorted; i++) // Iterates through the list so that the search begins after the largest number (the headnode found above). This stops the list from infinitely reordering itself by resorting previous headnodes.
		{
			prevNode = curNode;
			curNode = nextNode;
			nextNode = curNode->next;
		}

		newHead = curNode;
		curLargest = newHead->val; // Sets largest value to the value of the first unsorted node

		while (nextNode != 0) // Iterate through the rest of the list
		{
			if ((0 - nextNode->val) <= (0 - curLargest)) // Finding the largest unsorted number
			{
				prevNode = curNode;
				newHead = nextNode;
				curLargest = newHead->val;
			}

			// Iterating through list
			curNode = nextNode;
			nextNode = curNode->next;
		}

		// Sorting the list and resetting the variables, same as in the first pass
		if (newHead->next == 0)
		{
			prevNode->next = 0;
		}
		else
		{
			prevNode->next = newHead->next;
		}
		newHead->next = oldHead;
		oldHead = newHead;
		curNode = newHead;
		nextNode = curNode->next;
		prevNode = curNode;
		nodesSorted++;
	}

	return oldHead; // Return the headnode in the sorted list
}

// Testing code
//int main()
//{
//	Node* headNode = new Node;
//	headNode->val = 4;
//	headNode->next = new Node;
//	Node* curNode = headNode->next;
//	curNode->val = 7;
//	curNode->next = new Node;
//	curNode = curNode->next;
//	curNode->val = 6;
//	curNode->next = new Node;
//	curNode = curNode->next;
//	curNode->val = 2;
//	curNode->next = 0;
//
//	curNode = headNode;
//	while (curNode != 0)
//	{
//		printf("%d\n", curNode->val);
//		curNode = curNode->next;
//	}
//
//	headNode = sortList(headNode);
//
//	printf("\n");
//	curNode = headNode;
//	while (curNode != 0)
//	{
//		printf("%d\n", curNode->val);
//		curNode = curNode->next;
//	}
//}