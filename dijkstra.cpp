#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <cstddef>
#include <vector>
class node{
	public:
		class connection{
				public:
					node* neighbor;
					int distance;
					connection(node* new_neighbor, int new_distance){
							this -> neighbor = new_neighbor;
							this -> distance = new_distance;
					}
					int get_distance(){
						return this -> distance;
					}
					int get_neighbor_id(){
						return (*(this -> neighbor)).id;
					}
		};
		class connection_list{
			public:
					class connection_node{
						public:
							connection* self;
							connection_node* next;
							connection_node(connection* self){
								this -> self = self;
								this -> next = NULL;
							}
							bool next_is_null(){
								if(this -> next == NULL){
									return true;
								}else{
									return false;
								}
							}
							bool return_false(){
								return false;
							}
					};
				connection_node* first;
				connection_list(){
						this -> first = NULL;
				}
				void append(connection* new_connection){
					if(this -> first == NULL){
						connection_node* new_neighbor = (new connection_node(new_connection));
						this -> first = new_neighbor;
						return;
					}
					connection_node* current_connection = this -> first;
					while((*current_connection).next != NULL){
						current_connection = (*current_connection).next;
					}
					connection_node* new_node = (new connection_node(new_connection));
					(*current_connection).next = new_node;
				}
				connection get_connection(int index){
					connection_node current_connection = *first;
					for(int i = 0; i < index; i++){
						current_connection = *(current_connection.next);
					}
					return(*(current_connection.self));
				}
				int len(){
					if(this -> first == NULL){
						return 0;
					}else{
						connection_node* current_connection = (this -> first);
						int count = 1;
						while(!((*current_connection).next_is_null())){
							count ++;
							current_connection = ((*current_connection).next);
						}
						return(count);
					}
				}
		};
		int id;
		connection_list neighbors;
		node(int ident, connection_list cons){
			this -> id = ident;
			this -> neighbors = cons;
		}
		void connect(node* neighbor, int distance){
			connection* forward_connection = new connection(neighbor, distance);
			connection* backward_connection = new connection(this, distance);
			this -> neighbors.append(forward_connection);
			(*neighbor).neighbors.append(backward_connection);
		}
		void one_way(node* neighbor, int distance){
			connection* forward_connection = new connection(neighbor, distance);
			this -> neighbors.append(forward_connection);
		}
		int get_id(){
			return this -> id;
		}
		void print(){
			std::cout << "Node ID: ";
			std::cout << this -> id;
			std::cout << "\nConnections (neighbor:distance):";
			for(int i = 0; i < (this -> neighbors.len()); i++){
				std::cout << " ";
				connection working_connection = this -> neighbors.get_connection(i);
				std::cout << (working_connection).get_neighbor_id();
				std::cout << ":";
				std::cout << (working_connection).get_distance();
			}
			std::cout << "\n";
		}
};
int num_lines(std::string location){
	int count = 0;
	std::ifstream input;
	std::string line;
	input.open(location);
	if(input.is_open()){
		while(getline(input, line)){
			count++;
		}
	}
	return count;
}
int find_right(int left, std::string line){
	int right = left;
	for(int i = left; line[i + 1] != ' ' and (line[i + 1]) != '\0'; i++){
		right++;
	}
	return right;
}
int find_left(int right, std::string line){
	char tester = line[right + 1];
	if(tester != '\0'){
		return right + 2;
	}else{
		return right;
	}
}
int find_colon(int left, int right, std::string line){
	if(left == right){
		return 0;
	}
	while(line[left] != ':'){
		left++;
	}
	return left;
}
node* generate_graph(std::string location){
	int num_nodes = num_lines(location);
	int num_created = 0;
	node* node_arr[num_nodes];
	std::ifstream input;
	std::string line;
	input.open(location);
	if(input.is_open()){
		while(getline(input, line)){
			int new_identity = line[0] - '0';
			std::cout << "making node id: ";
			std::cout << new_identity;
			std::cout << "\n";
			node_arr[num_created] = new node(new_identity, node::connection_list());;
			num_created++;
		}
	}
	input.clear();
	input.seekg(0);
	int current = 0;
	if(input.is_open()){
		while(getline(input, line)){
			int left = 2;
			int right;
			int colon;
			right = find_right(left, line);
			colon = find_colon(left, right, line);
			std::cout << line;
			std::cout << "\n";
			while(left != right){
				int neighbor = stoi(line.substr(left, (colon-left)));
				int distance = stoi(line.substr(colon + 1, (left-colon)));
				left = find_left(right, line);
				right = find_right(left, line);
				colon = find_colon(left, right, line);
				(*(node_arr[current])).one_way(node_arr[neighbor], distance);
			}
			current++;
		}
	}else{
		std::cout << "Failed to read graph input (file not found)\n";
	}
	input.close();
	std::cout << "Number of generated nodes: ";
	std::cout << num_created;
	std::cout << "\n";
	return node_arr[0];
}
class lookup_list{
	public:
		int distance;
		int num_jumps;
		bool visited;
		std::vector<int> path;
		lookup_list* next;
		lookup_list* prev;
		node* subject;
		lookup_list(int input, node* input2){
			this -> distance = input;
			this -> next = NULL;
			this -> prev = NULL;
			this -> path = {};
			this -> visited = false;
			this -> subject = input2;
			this -> num_jumps = 0;
		}
		void append(lookup_list* new_item){
			if(this -> next == NULL){
				this -> next = new_item;
				(new_item) -> prev = this;
				return;
			}
			else{
				(*(this -> next)).append(new_item);
			}
		}
		lookup_list* get_last(){
			lookup_list* current_item = this;
			while((*current_item).next != NULL){
				current_item = (*current_item).next;
			}
			return current_item;
		}
		lookup_list* get_first(){
			lookup_list* current_item = this;
			while((*current_item).prev != NULL){
				current_item = (*current_item).prev;
			}
			return current_item;
		}
		lookup_list* get_index(int index){
			lookup_list* current_item = (*this).get_first();
			for(int i = 0; i < index; i++){
				current_item = (*current_item).next;
			}
			return current_item;
		}
		void pop(){
			lookup_list* current_item = (*this).get_last();
			lookup_list fore_item = *((*current_item).prev);
			fore_item.next = NULL;
			delete (current_item);
		}
		int len(){
			int count = 1;
			lookup_list* current_item = (*this).get_first();
			while((*current_item).next != NULL){
				current_item = (*current_item).next;
				count++;
			}
			return count;
		}
		bool contains(node* target){
			lookup_list* current = this;
			current = (*this).get_first();
			while((*current).next != NULL){
				if((*current).subject == target){
					return true;
				}
				current = (*current).next;
			}
			return false;
		}
		lookup_list* lookup(node* target){
			lookup_list* current = this;
			current = (*this).get_first();
			while((*current).next != NULL){
				if((*current).subject == target){
					return current;
				}
				current = (*current).next;
			}
			//if this second return statement actually gets used, something has gone wrong
			return current;
		}
};
lookup_list* generate_lookup(node* root_pointer, int num_nodes){
	lookup_list* list_root = new lookup_list(0, root_pointer);
	(*list_root).num_jumps = 0;
	for(int i = 1; i < (num_nodes+1); i++){
		node* current_node = root_pointer + (sizeof(node*)/4) * i;
		(*list_root).append(new lookup_list(INT_MAX, current_node));
	}
	return list_root;
}
std::vector<int> dijkstra(node* root, node* target, lookup_list* lookup_root){
	if((*(*lookup_root).lookup(target)).visited == true){
		std::cout << "target was visited!";
		std::cout << "\n";
		(*(*lookup_root).lookup(target)).path.push_back((*target).id);
		return (*(*lookup_root).lookup(target)).path;
	}
	int min_distance = INT_MAX;
	int min_jumps = INT_MAX;
	node* current_node;
	for(int i = 0; i < (*lookup_root).len(); i++){
		lookup_list* current_item = (*lookup_root).get_index(i);
		if((*current_item).visited == false and (*current_item).distance <= min_distance){
			min_distance = (*current_item).distance;
			current_node = (*current_item).subject;
		}
	}
	(*(*lookup_root).lookup(current_node)).visited = true;
	std::cout << "visiting node: ";
	std::cout << (*current_node).id;
	std::cout << "\n";
	int tentative_distance;
	int comparison_distance;
	node* checking_node;
	for(int i = 0; i < (*current_node).neighbors.len(); i++){
		checking_node = (((*current_node).neighbors.get_connection(i)).neighbor);
		lookup_list* checker = (*lookup_root).lookup(checking_node);
		tentative_distance = (*checker).distance;
		comparison_distance = (((*current_node).neighbors.get_connection(i)).distance) + (*(*lookup_root).lookup(current_node)).distance;
		if(comparison_distance < tentative_distance){
			(*checker).distance = comparison_distance;
			(*checker).path = std::vector<int>((*(*lookup_root).lookup(current_node)).path);
			(*checker).path.push_back((*current_node).id);
			(*checker).num_jumps = (*checker).path.size();
		}else if(comparison_distance == tentative_distance and (*checker).path.size() > (*(*lookup_root).lookup(current_node)).path.size() + 1){
			(*checker).path = std::vector<int>((*(*lookup_root).lookup(current_node)).path);
			(*checker).path.push_back((*current_node).id);
			(*checker).num_jumps = (*checker).path.size();
		}
	}
	lookup_list current_lookup = *lookup_root;
	for(int i = 0; i < (*lookup_root).len() - 1; i++){
		std::cout << "node: ";
		std::cout << i;
		std::cout << " current distance (from root): ";
		std::cout << current_lookup.distance;
		std::cout << " current jumps: ";
		std::cout << current_lookup.num_jumps;
		std::cout << "\n";
		current_lookup = *(current_lookup.next);
	}
	std::cout << "next round";
	std::cout << "\n";
	return dijkstra(root, target, lookup_root);
}
int main(){
	int num_nodes;
	num_nodes = num_lines("./saved_map");
	node* root_pointer = generate_graph("./saved_map");
	node* current_node;
	//these commented lines are useful for debugging the input file parsing function
	//for(int i = 0; i < num_nodes; i++){
	//	current_node = root_pointer + (sizeof(node*)/4) * i;
	//	(*current_node).print();
	//	std::cout << "\n";
	//	std::cout << "\n";
	//}
	lookup_list* lookup_root = generate_lookup(root_pointer, num_nodes);
	//next line determines target node for dijkstra's algorithm
	int target_id = 3;
	node* target_node = (root_pointer + (sizeof(node*)/4) * target_id);
	std::vector<int> traversal_path = dijkstra(root_pointer, target_node, lookup_root);
	std::cout << "target node: ";
	std::cout << (*target_node).id;
	std::cout << "\n";
	for(int i = 0; i < traversal_path.size(); i++){
		std::cout << traversal_path[i];
		if(i < traversal_path.size() - 1){
			std::cout << " -> ";
		}
	}
	std::cout << "\n";
	std::cout << "total distance: ";
	std::cout << (*(*lookup_root).lookup(target_node)).distance;
	std::cout << "\n";
}
