#include<iostream>
#include<string>
#include<sstream>
#include<list>
using namespace std;

class Token {
public:
	enum Kind {
		OP,
		
		PLUS,
		MINUS,
		
		EOF1,
		UNKNOWN
	};
private:
	Kind kind_;
	double value_;
public: 
	Token(Kind kind, double value = 0.0)
	:kind_(kind), value_(value)
	{}
	
	Kind get_kind() const {
		return kind_;
	}
	
	double get_value() const {
		return value_;
	}

};

ostream& operator<<(ostream& out, const Token& token) {
	switch(token.get_kind()) {
	case Token::OP:
		out << "T(OP, " <<token.get_value() << ")";
		break;
	case Token::PLUS:
		out << "T(PLUS)" ;
		break;
	case Token::MINUS:
		out <<"T(MINUS)";
		break;
	case Token::EOF1:
		out <<"T(EOF)";
		break;
	default:
		out <<"T(UNKNOWN)";
		break;
	}
	return out;
}

class Tokenizer {
	istream& in_;
public:
	Tokenizer(istream& in)
	: in_(in)
	{}
	
	Token next_token();

};

Token Tokenizer::next_token() {
	string word;
	in_ >> word;
	if (!in_) {
		return Token(Token::EOF1);
	}
	
	if (word == "+") {
		return Token(Token::PLUS);
	} else if (word == "-") {
		return Token(Token::MINUS);
	} else {
		double val = 0.0;
		istringstream istr(word);
		istr >> val;
		if(!istr) {
			return Token(Token::UNKNOWN);
		} else {
			return Token(Token::OP, val);
		}
	}
}


int main() {
	/*Token t1(Token::OP,3.14), t2(Token::PLUS);
	cout << t1 << endl;
	cout << t2 << endl;*/
	
	list<double> context;
	
	Tokenizer tin(cin);
	Token token = tin.next_token();
	while(token.get_kind() != Token::EOF1	) {
		cout << token << endl;
		switch(token.get_kind()) {
		case Token::OP:
			context.push_back(token.get_value());
			break;
		case Token::PLUS:
		{
			double v2 = context.back();
			context.pop_back();
			double v1 = context.back();
			context.pop_back();
			double result = v1 + v2;
			context.push_back(result);
			cout << "RESULT:" << result << endl;
		}
			break;
		case Token::MINUS:
		{
			double v2 = context.back();
			context.pop_back();
			double v1 = context.back();
			context.pop_back();
			double result = v1 - v2;
			context.push_back(result);
			cout << "RESULT:" << result << endl;
		}
			break;
		default:
			break;
		}
		
		token = tin.next_token();
	}
	
	return 0;
}
