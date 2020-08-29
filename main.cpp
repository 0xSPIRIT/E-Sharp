#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// Pointer Stuff.
#define TOKEN_SET_POINTER '@'
#define TOKEN_NEXT_INDEX '>'
#define TOKEN_PREV_INDEX '<'
#define TOKEN_REFERENCE_INDEX '&'
#define TOKEN_CURRENT_INDEX '~'

// Maths Operators.
#define TOKEN_ADD '+'
#define TOKEN_SUB '-'
#define TOKEN_MULT '*'
#define TOKEN_DIV '/'
#define TOKEN_MOD '%'
#define TOKEN_EQUALS '='

// Logic Operators.
#define TOKEN_IF '?'
#define TOKEN_ELSE ':'
#define TOKEN_IF_OPEN '{'
#define TOKEN_IF_CLOSE '}'

// Loops.
#define TOKEN_START_LOOP '['
#define TOKEN_END_LOOP ']'

// I/O
#define TOKEN_PRINT ';'
#define TOKEN_PRINT_ASCII ','
#define TOKEN_INPUT '$'
#define TOKEN_LOAD_FILE_START '('
#define TOKEN_LOAD_FILE_END   ')'

#define TOKEN_COMMENT '#'

#define MEM_SIZE 1000000

// Might change to std::vector later.
float memory[MEM_SIZE] = {0};
int ind = 0;

int interpret(vector<string> &lines);
void tokenize(vector<string> &lines, vector<string> &tokens);

bool is_float(const string &str);

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " program.es\n";
	return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Unable to open \"" << argv[1] << "\"!\n";
	return 1;
    }

    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    
    interpret(lines);
    return 0;
}

int interpret(vector<string> &lines) {
    vector<string> tokens;
    tokenize(lines, tokens);

    vector<string> functions;

    bool isinif = false;

    for (size_t i = 0; i < tokens.size(); i++) {
	string &tok = tokens[i];
	
        if (is_float(tok)) {
	    memory[ind] = stof(tok);
	} else if (tok[0] == TOKEN_ADD) {
	    if (tokens[i+1][0] == TOKEN_REFERENCE_INDEX) {
		if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
		    int in = stoi(tokens[i+=2]);
		    memory[ind] += memory[in];
		} else {
                    memory[ind] += memory[ind];
		}
	    } else {
                memory[ind]++;
	    }
	} else if (tok[0] == TOKEN_SUB) {
	    if (tokens[i+1][0] == TOKEN_REFERENCE_INDEX) {
		if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
		    int in = stoi(tokens[i+=2]);
		    memory[ind] -= memory[in];
		} else {
                    memory[ind] = 0;
		}
	    } else {
                memory[ind]--;
	    }
	} else if (tok[0] == TOKEN_MULT) {
	    if (tokens[i+1][0] == TOKEN_REFERENCE_INDEX) {
		if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
		    int in = stoi(tokens[i+=2]);
		    memory[ind] *= memory[in];
		} else {
                    memory[ind] *= memory[ind];
		}
	    } else {
                memory[ind] *= memory[ind];
	    }
	} else if (tok[0] == TOKEN_DIV) {
	    if (tokens[i+1][0] == TOKEN_REFERENCE_INDEX) {
		if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
		    int in = stoi(tokens[i+=2]);
		    memory[ind] /= memory[in];
		} else {
                    memory[ind] = 1;
		}
	    } else {
                memory[ind] = 1;
	    }
	} else if (tok[0] == TOKEN_MOD) {
	    int in = stoi(tokens[i+=2]);
	    memory[ind] = (int) memory[ind] % (int) memory[in];
	} else if (tok[0] == TOKEN_EQUALS) {
	    int in = stoi(tokens[i+=2]);
	    memory[ind] = memory[in];
	} else if (tok[0] == TOKEN_START_LOOP) {
	    if (!memory[ind]) {
                int opened = 1;
		while (opened) {
		    i++;
		    if (tokens[i][0] == TOKEN_START_LOOP) opened++;
		    if (tokens[i][0] == TOKEN_END_LOOP)   opened--;
		}
	    }
	} else if (tok[0] == TOKEN_END_LOOP) {
	    if (memory[ind]) {
                int opened = 1;
		while (opened) {
		    i--;
		    if (tokens[i][0] == TOKEN_START_LOOP) opened--;
		    if (tokens[i][0] == TOKEN_END_LOOP)   opened++;
		}
	    }
	} else if (tok[0] == TOKEN_PRINT) {
            cout << memory[ind] << endl;
	} else if (tok[0] == TOKEN_PRINT_ASCII) {
            putchar(memory[ind]);
	} else if (tok[0] == TOKEN_INPUT) {
	    float num;
	    cin >> num;
	    if (tokens[i+1][0] != TOKEN_CURRENT_INDEX) {
		memory[stoi(tokens[++i])] = num;
	    } else {
                memory[ind] = num;
	    }
	} else if (tok[0] == TOKEN_SET_POINTER) {
	    if (tokens[i+1][0] != TOKEN_CURRENT_INDEX) {
                ind = stof(tokens[++i]);
	    }
	} else if (tok[0] == TOKEN_NEXT_INDEX) {
            ++ind;
	} else if (tok[0] == TOKEN_PREV_INDEX) {
            --ind;
	} else if (tok[0] == TOKEN_IF) {
	    float val1, val2;
	    if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
                val1 = memory[stoi(tokens[i+=2])];
	    } else {
                val1 = memory[ind];
	    }
	    if (tokens[i+2][0] != TOKEN_CURRENT_INDEX) {
                val2 = memory[stoi(tokens[i+=2])];
	    } else {
                val2 = memory[ind];
	    }

	    if (val1 != val2) {
		i++;
                // Go to else statement, if it exists.
		int opened = 1;
		while (opened) {
                    i++;
		    if (tokens[i][0] == TOKEN_IF_OPEN)  opened++;
		    if (tokens[i][0] == TOKEN_IF_CLOSE) opened--;
		}

		if (tokens[i+1][0] == TOKEN_ELSE) {
                    i += 2;
		}
	    } else {
                isinif = true;
	    }
	} else if (isinif && tok[0] == TOKEN_IF_CLOSE) {
	    if (tokens[i+1][0] != TOKEN_ELSE) continue;
	    
            i += 2;
	    int opened = 1;
	    bool had_nested_ifs = false;
	    while (opened) {
                i++;
		if (tokens[i][0] == TOKEN_IF_OPEN) { opened++; had_nested_ifs = true; }
		if (tokens[i][0] == TOKEN_IF_CLOSE)  opened--;
	    }

	    if (!had_nested_ifs) {
                isinif = false;
	    }
	} else if (tok[0] == TOKEN_LOAD_FILE_START) {
            string file = "";
	    for (int c = 1; c < tok.size(); c++) {
		if (tok[c] == TOKEN_LOAD_FILE_END) break;
		
                file += tok[c];
	    }
	    ifstream stream(file);
	    string line, content;
	    if (stream.is_open()) {
                while (getline(stream, line)) {
                    content += line;
		    content += '\n';
		}
		for (char c: content) {
                    memory[ind++] = (float) c;
		}
	    }
	}
    }
    
    return 0;
}

void tokenize(vector<string> &lines, vector<string> &tokens) {
    for (string &str: lines) {
        string token = "";
        bool is_comment = false;
	bool is_in_file_string = false;
        for (char c: str) {
            if (c == TOKEN_COMMENT) {
		is_comment = true;
		break;
	    } else if (c == TOKEN_LOAD_FILE_START) {
                is_in_file_string = true;
	    }

	    if (is_in_file_string) {
                token += c;
		if (TOKEN_LOAD_FILE_END) {
                    is_in_file_string = false;
		    continue;
		}
		continue;
	    }

	    // Ugly (maybe have an enum, then iterate through that?)
            if (c == TOKEN_SET_POINTER ||
		c == TOKEN_REFERENCE_INDEX ||
		c == TOKEN_ADD ||
		c == TOKEN_SUB ||
		c == TOKEN_MULT ||
		c == TOKEN_DIV ||
		c == TOKEN_MOD ||
		c == TOKEN_EQUALS ||
		c == TOKEN_IF ||
		c == TOKEN_ELSE ||
		c == TOKEN_IF_OPEN ||
		c == TOKEN_IF_CLOSE ||
		c == TOKEN_NEXT_INDEX ||
		c == TOKEN_PREV_INDEX ||
		c == TOKEN_CURRENT_INDEX ||
		c == TOKEN_START_LOOP ||
		c == TOKEN_LOAD_FILE_START ||
		c == TOKEN_LOAD_FILE_END ||
		c == TOKEN_END_LOOP ||
		c == TOKEN_INPUT ||
		c == TOKEN_PRINT ||
		c == TOKEN_PRINT_ASCII ||
		c == ' ') {
		if (token != "") {
		    tokens.push_back(token);
		    token = "";
		}
		
		if (c != ' ') {
		    string t; t += c;
		    tokens.push_back(t);
		}
	    } else {
                token += c;
	    }
	}
        if (is_comment) continue;
	
	if (token != "") {
            tokens.push_back(token);
	}
	token = "";
    }
}

bool is_float(const string &str) {
    bool is_float = false;
    try {
	stof(str);
	is_float = true;
    } catch (invalid_argument) { return false; }

    return is_float;
}
