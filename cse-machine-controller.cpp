#include "header.h"

string const bi_operators[] = {"**", "+", "-", "*", "/", "<", ">", "<=", ">=", "or", "and", "eq", "gr", "ge", "ls", "le", "ne", "aug", "|"};
unordered_set<string> bi_ops(bi_operators, bi_operators + sizeof(bi_operators) / sizeof(bi_operators[0]));

string const un_operators[] = {"not", "neg"};
unordered_set<string> un_ops(un_operators, un_operators + sizeof(un_operators) / sizeof(un_operators[0]));

void start_machine (unordered_map<string, cseNode>* envs[], stack<cseNode*> c_control, stack<cseNode*> s_stack, queue<cseNode*>* deltas[]) {
	cseNode* temp;

	while (!c_control.empty()) {

		// cout << "\n ------Iteration -------\n";
		
		temp = c_control.top();

		//if INT, add it to the stack as it is
		if (isInt(temp->name) || temp->type == "func" || isStr(temp->name)) {
			s_stack.push(temp);
			c_control.pop();
		}

		//if boolean, simply transfer to stack
		else if (temp->name == "<true>" || temp->name == "<false>") {
			temp->type = "bool";
			s_stack.push(temp);
			c_control.pop();
		}

		//if popped node is a lambda
		else if (temp->name == "lambda") {
			temp->k = current_env.top();
			s_stack.push(temp);
			c_control.pop();
		}
		//if its a BINARY operator
		else if (bi_ops.count(temp->name)) {
			
			string rand1 = s_stack.top()->name; s_stack.pop();	//get rand1
			string rand2 = s_stack.top()->name;	s_stack.pop();	//get rand2

			cseNode *n = bi_operation(temp->name, rand1, rand2);
			s_stack.push(n);

			c_control.pop();	//pop the operator
		}

		//is a UNARY operator
		else if (un_ops.count(temp->name)) {
			string rand1 = s_stack.top()->name; s_stack.pop();	//get operand from stack
			cseNode *n = un_operation (temp->name, rand1);
			s_stack.push(n);
			c_control.pop();
		}

		//if gamma and lambda pair
		else if (temp->name == "gamma" && s_stack.top()->name == "lambda") {
			c_control.pop();	//pop gamma
			cseNode* lambda = s_stack.top(); s_stack.pop();	//pop and get lambda
			cseNode* value_node = s_stack.top(); s_stack.pop(); 	//pop and get rand

			cseNode* newEnv = createNextEnv (envs, s_stack.top()->k);	//new env node to add to stack and control
			c_control.push(newEnv);	//add env to control and stack
			s_stack.push(newEnv);

			// -- update current env -----
			unordered_map<string, cseNode> *currentEnv = envs[env_count-1]; 	//get current env

			//update current environment
			if ( value_node->type == "tuple") {
				
				string tup = value_node->name;  //get the VALUE tuple
				tup = tup.substr(1, tup.length()-2);

				stringstream ss(tup);		//split the tuple on comma
				vector<string> split_tuple;
				while (ss.good()) {
					string substr;
			    	getline( ss, substr, ',' );
			    	split_tuple.push_back( substr );
				}

				string x_array = lambda->x;	//get KEY array 

				stringstream ss1(x_array);		//split the tuple on comma
				vector<string> x_tuple;
				while (ss1.good()) {
					string substr2;
			    	getline( ss1, substr2, ',' );
			    	x_tuple.push_back( substr2 );
				}

				for (int i=0; i < x_tuple.size(); i++) {
					string value1 = split_tuple[i];
					value1.erase(value1.begin(), std::find_if(value1.begin(), value1.end(), std::bind1st(std::not_equal_to<char>(), ' '))); //remove trailing spaces
					(*currentEnv)[extractID(x_tuple[i])] = *newCSENode(value1, "");	//add to env, each pair
				}
			}
			else {
				string id_ = extractID(lambda->x);
				(*currentEnv)[id_] = *value_node;
			}

			//---load delta onto the control
			load_control (lambda->i, c_control, deltas);
		}

		//If gamma and Print
		else if (temp->name == "gamma" && s_stack.top()->name == "<ID:Print>") {
			c_control.pop();	//pop th gamma
			s_stack.pop();	//pop the print node
			string str = s_stack.top()->name;
			myPrint(str);

			cseNode* dummy = newCSENode ("<dummy>", "dummy");
			s_stack.pop();	//pop the node to be printed
			s_stack.push(dummy);
		}

		else if (temp->name == "gamma" && s_stack.top()->type == "tuple") {

			c_control.pop();	//pop the gamma
			string tuple = s_stack.top()->name;	//get the tuple
			s_stack.pop();		//pop tuple from stack

			tuple = tuple.substr(1, tuple.length()-2);	//remove brackets from tuple
			//split on , and add to vector
			stringstream ss(tuple);
			vector<string> result;
			while (ss.good()) {
				string substr;
			    getline( ss, substr, ',' );
			    substr.erase(substr.begin(), std::find_if(substr.begin(), substr.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
			    result.push_back( substr );
			}

			string num = s_stack.top()->name;
			s_stack.pop(); 		//remove the number
			int index = extractInt(num);
			
			string fetched_value = result[index-1];		//rpal indices start from 1

			cseNode* result_node = newCSENode(fetched_value, "");
			s_stack.push(result_node);
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

		else if (temp->name == "beta") {

			c_control.pop();	//pop off beta from control
			
			int deltaIndex; 	//to get the subscript of the delta node

			string b = s_stack.top()->name;
			
			if (b == "<false>") {
				
				cseNode* del = c_control.top();

				if (del->type == "false") {
					deltaIndex = del->i;
					c_control.pop();
					c_control.pop();
					s_stack.pop();
					load_control(deltaIndex, c_control, deltas);
				}
				else {
					cout << "ERROR : Expecting a false delta ttype in conditional operator";
				}
			}
			else if (b == "<true>") {
				c_control.pop();
				cseNode* del = c_control.top();
				deltaIndex = del->i;
				c_control.pop();
				s_stack.pop();
				load_control(deltaIndex, c_control, deltas);
			}

		}

		//if next node is a delta (TAU's child node type)
		//we just load the control structure onto the control
		else if (temp->name == "delta" && temp->type == "delta") {
			int deltaIndex = temp->i;
			c_control.pop();
			load_control(deltaIndex, c_control, deltas);
		}

		else if (temp->name == "tau") {
			
			int tauSize = temp->i;
			string tuple = "()";	//create empty tuple;
			
			for (int i=0; i < tauSize; i ++) {		//aug to tuple n nodes popped from stack
				tuple = augToTuple(tuple, s_stack.top()->name);
				s_stack.pop();
			}

			cseNode* tup = new cseNode;	//add the tuple to the stack
			tup->name = tuple;
			tup->type = "tuple";
			tup->i = tauSize;
			s_stack.push(tup);

			c_control.pop(); 	//pop the tau from control
		}
		else {
			c_control.pop();
		}
		// cout << "Control : \n";  printStack(c_control);
		// cout << "Stack : \n";    printStack(s_stack);
	}
	cout <<"\n";
}

string augToTuple (string tuple, string val) {

	if (tuple == "()" ) {		//if empty tuple
		return "(" + val + ")";
	}
	else {
		// return "(" + val + ", " + tuple.substr(1);
		return tuple.substr(0, tuple.length()-1) + ", " + val + ")";
	}
}

bool isInt (string name) {
	return name.find("<INT:") != std::string::npos;
}

bool isID (string name) {
	return name.find("<ID:") != std::string::npos;
}

bool isStr (string name) {
	return name.find("<STR:") != std::string::npos;
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

string extractStr (string name) {
	return name.substr(6, name.length()-8);
	//TODO : take care of \t and \n
} 

cseNode* bi_operation (string op, string rand1, string rand2) {	//TODO: complete
	//rand1 op rand2

	if (op == "+") {
		int result = extractInt(rand1) + extractInt(rand2);
		cseNode *n = newCSENode (to_string(result), "int");
		n->name = "<INT:" + n->name + ">";
		return n;
	}
	else if (op == "-") {
		int result = extractInt(rand1) - extractInt(rand2);
		cseNode *n = newCSENode (to_string(result), "int");
		n->name = "<INT:" + n->name + ">";
		return n;
	}
	else if (op == "*") {
		int result = extractInt(rand1) * extractInt(rand2);
		cseNode *n = newCSENode (to_string(result), "int");
		n->name = "<INT:" + n->name + ">";
		return n;
	}
	else if (op == "/") {
		int result = extractInt(rand1) / extractInt(rand2);
		cseNode *n = newCSENode (to_string(result), "int");
		n->name = "<INT:" + n->name + ">";
		return n;
	}
	else if (op == "**") {
		int result = pow(extractInt(rand1), extractInt(rand2));
		cseNode *n = newCSENode (to_string(result), "int");
		n->name = "<INT:" + n->name + ">";
		return n;
	}
	else if (op == "<" || op == "ls") {
		cseNode *n;
		if (extractInt(rand1) < extractInt(rand2)) {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == "<=" || op == "le") {
		cseNode *n;
		if (extractInt(rand1) <= extractInt(rand2)) {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == ">" || op == "gr") {
		cseNode *n;
		if (extractInt(rand1) > extractInt(rand2)) {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == ">=" || op == "ge") {
		cseNode *n;
		if (extractInt(rand1) >= extractInt(rand2)) {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == "ne") {
		cseNode *n;
		if (extractInt(rand1) == extractInt(rand2)) {
			n = newCSENode ("<false>", "bool");
		} else {
			n = newCSENode ("<true>", "bool");
		}
		return n;
	}
	else if (op == "eq") {
		cseNode *n;
		if (extractInt(rand1) == extractInt(rand2)) {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == "and") {
		cseNode *n;
		if (rand1 == "<true>" && rand2 == "<true>") {
			n = newCSENode ("<true>", "bool");
		} else {
			n = newCSENode ("<false>", "bool");
		}
		return n;
	}
	else if (op == "or") {
		cseNode *n;
		if (rand1 == "<false>" && rand2 == "<false>") {
			n = newCSENode ("<false>", "bool");
		} else {
			n = newCSENode ("<true>", "bool");
		}
		return n;
	}
	return NULL;
}

cseNode* un_operation (string op, string rand1) {

	if (op == "neg") {
		int rand2 = extractInt(rand1);
		rand2 = -rand2;
		return newCSENode ("<INT:" + to_string(rand2) + ">", "int");
	}
	else if (op == "not") {
		if (rand1 == "<true>") {
			return newCSENode ("<false>", "bool");
		}
		else {
			return newCSENode ("<true>", "bool");
		}
	}
	else {
		cout << "ERROR : unary operation not valid";
		return NULL;
	}
}

void myPrint (string name) {
	if (isInt(name)) {
		cout << extractInt(name);
	}
	else if (isID(name)) {
		cout << extractID(name);
	}
	else if (isStr(name)) {
		cout << extractStr(name);
	}
}



















