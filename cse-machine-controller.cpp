#include "header.h"

string const bi_operators[] = {"+", "-", "*", "/", "<", ">", "<=", ">=", "or", "and", "eq", "gr", "ge", "ls", "le", "ne", "aug", "|"};
unordered_set<string> bi_ops(bi_operators, bi_operators + sizeof(bi_operators) / sizeof(bi_operators[0]));

string const un_operators[] = {"not", "**", "neg", "!"};
unordered_set<string> un_ops(un_operators, un_operators + sizeof(un_operators) / sizeof(un_operators[0]));

void start_machine (unordered_map<string, cseNode>* envs[], stack<cseNode*> c_control, stack<cseNode*> s_stack, queue<cseNode*>* deltas[]) {
	cseNode* temp;
	int z=1;

	while (!c_control.empty() && z<10) {

		cout << "\n ------Iteration " << z <<"-------\n";
		
		temp = c_control.top();

		//if INT, add it to the stack as it is
		if (isInt(temp->name)) {
			s_stack.push(temp);
			c_control.pop();
		}

		//if popped node is a lambda
		else if (temp->name == "lambda") {
			temp->k = current_env.top();
			s_stack.push(temp);
			c_control.pop();
		}
		//if its a binary operator
		else if (bi_ops.count(temp->name)) {
			
			string rand1 = s_stack.top()->name; s_stack.pop();	//get rand1
			string rand2 = s_stack.top()->name;	s_stack.pop();	//get rand2

			cseNode *n = bi_operation(temp->name, rand1, rand2);
			n->name = "<INT:" + n->name + ">";

			s_stack.push(n);

			c_control.pop();	//pop the operator
		}

		//if gamma and lambda pair
		else if (temp->name == "gamma" && s_stack.top()->name == "lambda") {
			c_control.pop();	//pop gamma
			cseNode* lambda = s_stack.top(); s_stack.pop();	//pop and get lambda
			cseNode* value_node = s_stack.top(); s_stack.pop(); 	//pop and get rand

			cseNode* newEnv = createNextEnv (envs);	//new env node to add to stack and control
			c_control.push(newEnv);	//add env to control and stack
			s_stack.push(newEnv);

			//update current environment
			string id_ = extractID(lambda->x);
			unordered_map<string, cseNode> *currentEnv = envs[env_count-1];
			(*currentEnv)[id_] = *value_node;

			//load delta onto the control
			load_control (lambda->i, c_control, deltas);
		}

		//if a variable is popped
		else if (isID (temp->name)) {
			
			unordered_map<string, cseNode> *currentEnv = envs[current_env.top()];	//get current env

			string id = extractID(temp->name);	//get id from <ID: > node
			
			unordered_map<string, cseNode>::iterator p;
  
			// find value given key
			p = (*currentEnv).find(id);
			if(p != (*currentEnv).end()) {
				cseNode* cn = &p->second;
				s_stack.push(cn);
			  }    
			else{
				cout << "ERROR : Key not in map.\n";
			}
			c_control.pop();
		}
		// if its an env variable
		else if (temp->type == "env") {
			
			current_env.pop();	//update current env
			
			cseNode* cn = s_stack.top();	//store value popped from stack
			s_stack.pop();

			c_control.pop();	//pop env from control
			s_stack.pop();		//pop env from stack

			s_stack.push(cn);	//put back cn
		}



		cout << "Control : \n";  printStack(c_control);
		cout << "Stack : \n";    printStack(s_stack);
		z++;	//remove z after testing
	}
}

bool isInt (string name) {
	return name.find("<INT:") != std::string::npos;
}

bool isID (string name) {
	return name.find("<ID:") != std::string::npos;
}

string extractID (string name) {
	if (isID(name)) {
		return name.substr(4, name.length()-5);
	}
	else {
		cout << "ERROR : can't find ID\n";
		return "";
	}
}

int extractInt (string name) {		//extracts x out of node of form <INT:x>
	if (isInt(name)) {
		string s = name.substr(5, name.length()-6);
		return stoi(s);
	}
	else {
		cout << "ERROR : NOT an int";
		return 0;
	}
}

cseNode* bi_operation (string op, string rand1, string rand2) {
	//rand1 op rand2
	if (op == "+") {
		int result = extractInt(rand1) + extractInt(rand2);
		cseNode *n = newCSENode (to_string(result), "INT");
		return n;
	}
	return NULL;
}



















