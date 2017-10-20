/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Parser body class to define grammar rules for
  non-terminal chunks of the code and build the tree to be assembled
*/

#include "parser.h"
#include <cstring>
#include <iostream>
using namespace std;

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
		      "AND", "OR",
		      "LOADL", "LOADV", "STORE",
		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
		      "PRINTF",
		      "LABEL", "SEQ",
		      "FUNC", "PARAM"
		      };

Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1),
					      tindex(1){
  token = lexer.nextToken();
}

Parser::~Parser() {
};

void Parser::error(string message) {
  cerr << message << " Found " << token->getLexeme() << " at line " <<
    token->getLine() << " position " << token->getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token->getTtype() != tokenType)
    error(message);
}
		     
Parser::TreeNode* Parser::factor() {
  switch (token->getTtype()){
  case Token::LPAREN:
    {
    token = lexer.nextToken();
    TreeNode* node = expression();
    check(Token::RPAREN, "Expected )");
    token = lexer.nextToken();
    return node;
    }
    break;
  case Token::INTLIT:
    //LOADL loads the INTLIT to the stack
    //Using the TreeNode constructor with op and val
    {
    TreeNode* node = new TreeNode(LOADL, token->getLexeme()); //231
    token = lexer.nextToken();
    return node;
    }
    break;
    //save the lexeme of the IDENT and read one token past to see if
    //it's a funcall if Token is an ident
  case Token::IDENT:
    {
      string lexeme = token->getLexeme();
      token = lexer.nextToken();
      if (token->getTtype() == Token::LPAREN){   //funcall
	int param = 0;
	token = lexer.nextToken();
	TreeNode* root;
	if(token->getTtype() != Token::RPAREN){ //(z+1)  one parameter
	  root = expression();
	  param++;
	}
	//cout << token->getLexeme();
	while(token->getTtype() == Token::COMMA){ //(z+1, 2)  multiple parameters
	  token = lexer.nextToken();
	  TreeNode* expression2 = expression();
	  root = new TreeNode(SEQ, root, expression2);
	  param++;
	}
	check(Token::RPAREN, "Expecting ')' ");
	token = lexer.nextToken();
	TreeNode* loadL = new TreeNode(LOADL, itos(param * 8));
	root = new TreeNode(SEQ, root, loadL);
	TreeNode* call = new TreeNode(CALL, lexeme);
	root = new TreeNode(SEQ, root, call);
	return root;
      }
      else{   //Just an IDENT
	int x = symTab.getUniqueSymbol(lexeme);
	if (x == -1){
	  error("Var  not declared");
	}
        TreeNode* node = new TreeNode(LOADV, itos(x)); //x
        return node;
      }
    }
    break;
 default:
   error("Expecting integer literal, variable, or (");
   return NULL;
  }
}

Parser::TreeNode* Parser::term() {
  TreeNode* root = factor();
  while(token->getTtype() == Token::TIMES || token->getTtype() == Token::DIVIDE){
    int tokentype = token->getTtype();
    token = lexer.nextToken();
    TreeNode* newnode = factor();
    switch (tokentype){
    case Token::TIMES:
	root = new TreeNode(MULT, root, newnode); //new would create *TreeNode
	break;
    case Token::DIVIDE:
      root = new TreeNode(DIV, root, newnode);  //recursively build
						//the tree by calling
						//root as a parameter
                                                //again in the TreeNode
      break;
    default:
      error("Expecting multiply or divide operator");
      return NULL;
    }
  }
  return root;
}

//Call nextToken() when you have a token but not when you have nonterminal
Parser::TreeNode* Parser::expression() {
  TreeNode* root = term();
  while (token->getTtype() == Token::PLUS || token->getTtype() == Token::MINUS){
    int tokentype = token->getTtype();
    token = lexer.nextToken();
    TreeNode* newnode = term();
    switch (tokentype){
    case Token::PLUS:
      root = new TreeNode(ADD, root, newnode);
      break;
    case Token::MINUS:
      root = new TreeNode(SUB, root, newnode);
      break;
    default:
      error("Expecting add or subtract operator");
      return NULL;
    }
  }
  return root;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* root = expression();
  if (token->getTtype() == Token::EQ || token->getTtype() == Token::LT || token->getTtype() == Token::LE ||
      token->getTtype() == Token::GT || token->getTtype() == Token::GE || token->getTtype() == Token::NE){
    int tokentype = token->getTtype();
    token = lexer.nextToken();
    TreeNode* newnode = expression();
    switch (tokentype){
    case Token::EQ:
      root = new TreeNode(ISEQ, root, newnode);
      break;
    case Token::LT:
      root = new TreeNode(ISLT, root, newnode);
      break;
    case Token::LE:
      root = new TreeNode(ISLE, root, newnode);
      break;
    case Token::GT:
      root = new TreeNode(ISGT, root, newnode);
      break;
    case Token::GE:
      root = new TreeNode(ISGE, root, newnode);
      break;
    case Token::NE:
      root = new TreeNode(ISNE, root, newnode);
      break;
    default:
      error("Expecting ==, <, <=, >, >=, or != ");
      return NULL;
    }
  }
  return root;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* root = relationalExpression();
  while (token->getTtype() == Token::AND || token->getTtype() == Token::OR){
    int tokentype = token->getTtype();
    token = lexer.nextToken();
    TreeNode* newnode = relationalExpression();
    switch (tokentype){
    case Token::AND:
      {
      root = new TreeNode(AND, root, newnode);
      }
      break;
    case Token::OR:
      {
      root = new TreeNode(OR, root, newnode);
      }
      break;
    default:
      error("Expecting && or || operators ");
      return NULL;
    }
  }
  return root;
}

Parser::TreeNode* Parser::vardefStatement() {
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting var name ");
  string lex1 = token->getLexeme();
  int x = symTab.addSymbol(lex1); //addSymbol
  if(x == 0){
    error("Trouble adding symbol");
  }
  token = lexer.nextToken();
  while(token->getTtype() == Token::COMMA){
    token = lexer.nextToken();
    check(Token::IDENT, "Expecting var name ");
    string lex2 = token->getLexeme();
    int x = symTab.addSymbol(lex2); //addSymbol
    if (x == 0){
      error("Trouble adding symbol");
    }
    token = lexer.nextToken();
  }
  check(Token::SEMICOLON, "Expecting ';' ");
  token = lexer.nextToken();
  TreeNode* root = new TreeNode(SEQ);
  return root;
}

//IDENT ASSIGN <logicalexpression> SEMICOLON
Parser::TreeNode* Parser::assignStatement() {
  check(Token::IDENT, "Expecting identifier ");
  string lexeme = token->getLexeme();
  //GetUniqueSymbol
  int x = symTab.getUniqueSymbol(lexeme);
  //check if symbol is in table
  if (x == -1){
    error("Var not declared");
  }
  token = lexer.nextToken();
  check(Token::ASSIGN, "Expecting = ");
  token = lexer.nextToken();
  TreeNode* root = logicalExpression();
  check(Token::SEMICOLON, "Expecting ; ");
  token = lexer.nextToken();
  TreeNode* variable = new TreeNode(STORE, itos(x)); 
  root = new TreeNode(SEQ, root, variable);
  return root;
} 

Parser::TreeNode* Parser::printfStatement() {
  TreeNode* paramList = NULL;
  int nparams = 0;
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token->getLexeme();
  token = lexer.nextToken();
  if (token->getTtype() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (token->getTtype() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  TreeNode* printStatement = new TreeNode(SEQ, paramList,
		  new TreeNode(PRINTF, itos(nparams) + formatString));
  return printStatement;
}


// WHILE LPAREN <logicalexpression> RPAREN <block>
Parser::TreeNode* Parser::whileStatement() {
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '(' ");
  token = lexer.nextToken();
  TreeNode* newnode = logicalExpression();
  check(Token::RPAREN, "Expecting ')' ");
  token = lexer.nextToken();
  TreeNode* blockNode = block();
  //Tree Generation
  string s1 = makeLabel(); //L1
  TreeNode* root = new TreeNode(LABEL, s1 + ": ");
  root = new TreeNode(SEQ, root, newnode);
  string s2 = makeLabel(); //L2
  TreeNode* jump1 = new TreeNode(JUMPF, s2);
  root = new TreeNode(SEQ, root, jump1);
  root = new TreeNode(SEQ, root, blockNode);
  TreeNode* jump2 = new TreeNode(JUMP, s1);
  root = new TreeNode(SEQ, root, jump2);
  TreeNode* label2 = new TreeNode(LABEL, s2 + ": ");
  root = new TreeNode(SEQ, root, label2);
  return root;
}

//IF LPAREN <logicalexpression> RPAREN <block> [ELSE <block>]
Parser::TreeNode* Parser::ifStatement() {
  //already checked in statement that the first token is IF
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '(' ");
  token = lexer.nextToken();
  TreeNode* newnode = logicalExpression();
  check(Token::RPAREN, "Expecting ')' ");
  token = lexer.nextToken();
  TreeNode* thenblock = block();
  if (token->getTtype() == Token::ELSE){
    token = lexer.nextToken();
    TreeNode* elseblock = block();
    //Tree Generation
    TreeNode* root = newnode;
    string s1 = makeLabel();
    TreeNode* jump1 = new TreeNode(JUMPF, s1);
    root = new TreeNode(SEQ, root, jump1);
    root = new TreeNode(SEQ, root, thenblock);
    string s2 = makeLabel();
    TreeNode* jump2 = new TreeNode(JUMP, s2);
    root = new TreeNode(SEQ, root, jump2);
    TreeNode* label1 = new TreeNode(LABEL, s1 + ": ");
    root = new TreeNode(SEQ, root, label1);
    root = new TreeNode(SEQ, root, elseblock);
    TreeNode* label2 = new TreeNode(LABEL, s2 + ": ");
    root = new TreeNode(SEQ, root, label2);
    return root;
  }
  //no else block
  else{
    TreeNode* root = newnode;
    string s1 = makeLabel();
    TreeNode* jump1 = new TreeNode(JUMPF, s1);
    root = new TreeNode(SEQ, root, jump1);
    root = new TreeNode(SEQ, root, thenblock);
    TreeNode* label1 = new TreeNode(LABEL, s1 + ": ");
    root = new TreeNode(SEQ, root, label1);
    return root;
  }
}

Parser::TreeNode* Parser::returnStatement() {
  token = lexer.nextToken();
  TreeNode* root = logicalExpression();
  check(Token::SEMICOLON, "Expecting ';' ");
  token = lexer.nextToken();
  TreeNode* ret = new TreeNode(RET);
  root = new TreeNode(SEQ, root, ret);
  return root;
}

Parser::TreeNode* Parser::statement() {
  switch (token->getTtype()){
      case Token::VAR:
    {
      TreeNode* newnode = vardefStatement();
      return newnode;
    }
    break;
  case Token::IDENT:
    {
      if(token->getLexeme() == "printf"){
      TreeNode* newnode = printfStatement();
      return newnode;
      }
      else{
      TreeNode* newnode = assignStatement();
      return newnode;
      }
    }
    break;
  case Token::WHILE:
    {
      TreeNode* newnode = whileStatement();
      return newnode;
    }
    break;
  case Token::IF:
    {
      TreeNode* newnode = ifStatement();
      return newnode;
    }
    break;
    case Token::RETURN:
    {
       TreeNode* newnode = returnStatement();
      return newnode;
      }
      break;
  default:
    error("Expecting if, return, var, or while keyword or identifier ");
    return NULL;
  }
}

Parser::TreeNode* Parser::block() {
  check(Token::LBRACE, "Expecting '{' ");
  symTab.enterScope(); //scope
  token = lexer.nextToken();
  TreeNode* root;
  root = statement();
  if(token->getTtype() == Token::RBRACE){
  token = lexer.nextToken();
  return root;
  }
  //tokentype is while, if, ident or current token is not equals to rBrace
  while(token->getTtype() != Token::RBRACE){
      TreeNode* statement1 = statement();
      root = new TreeNode(SEQ, root, statement1);
  }
  check(Token::RBRACE, "Expecting } ");
  symTab.exitScope(); //scope
  token = lexer.nextToken();
  return root;
}

Parser::TreeNode* Parser::parameterdefs() {
  TreeNode* param1;
  if(token->getTtype() == Token::IDENT){
    string lexeme = token->getLexeme();
    strStack.push(lexeme);
    token = lexer.nextToken();
    while(token->getTtype() == Token::COMMA){
      token = lexer.nextToken();
      check(Token::IDENT, "Expecting ident ");
      string lexmeme = token->getLexeme();
      strStack.push(lexmeme);
      token = lexer.nextToken();
    }
  }
  symTab.addSymbol(strStack.peek());
  //call getUniqueSymbol on the strings popped off the string stack
  //then convert it to a string through itos
  param1 = new TreeNode(PARAM, itos(symTab.getUniqueSymbol(strStack.pop())));
  while(!strStack.isEmpty()){
    symTab.addSymbol(strStack.peek());
    //Repeat the process of calling getUniqueSymbol on the strings
    //popped off the stack and SEQ it to the existing tree 
    param1 = new TreeNode(SEQ, param1, new TreeNode(PARAM, itos(symTab.getUniqueSymbol(strStack.pop()))));
  }
  return param1;
}

Parser::TreeNode* Parser::function() {
  symTab.resetCount();
  TreeNode* root;
  check(Token::FUNCTION, "Expecting keyword 'function' ");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting function's name ");
  string lexeme = token->getLexeme();
  TreeNode* func1 = new TreeNode(FUNC, lexeme);
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting '(' ");
  symTab.enterScope();
  token = lexer.nextToken();
  //function has parameters
  if (token->getTtype() == Token::IDENT){
    TreeNode* param = parameterdefs();
    check(Token::RPAREN, "Expecting ')' ");
    token = lexer.nextToken();
    TreeNode* block1 = block();
    //Needs Tree Generation
    //SEQ block1 as rightchild and a SEQ as leftchild
    //Leftchild has FUNC as its left and param as its right
    root = new TreeNode(SEQ, new TreeNode(SEQ, func1, param), block1);
    symTab.exitScope();
    return root;
  }
  //function has no parameters
  else{
  check(Token::RPAREN, "Expecting ')' ");
  token = lexer.nextToken();
  TreeNode* block1 = block();
  //param node is just a SEQ Treenode bc function has no parameters
  TreeNode* empty = new TreeNode(SEQ);
  root = new TreeNode(SEQ, new TreeNode(SEQ, func1, empty), block1);
  symTab.exitScope();
  return root;
  }
}

Parser::TreeNode* Parser::compilationunit() {
  TreeNode* root;
  if(token->getTtype() == Token::FUNCTION){
    //Don't need to do nextToken because function does it
    root = function();
  }
  while(token->getTtype() == Token::FUNCTION){
    TreeNode* func = function();
    root = new TreeNode(SEQ, root, func);
  }
  return root;
}

void Parser::emit(string s){
  cout << '\t' << s << endl;
}

void Parser::isOps(string s){
  string s1 = makeLabel();
  string s2 = makeLabel();
  emit("pop rbx");
  emit("pop rax");
  emit("cmp rax,rbx");
  emit(s + " " + s1);
  emit("mov rax,0");
  emit("jmp " + s2);
  emit(s1 + ":");
  emit("mov rax,1");
  emit(s2 + ":");
  emit("push rax");
}
		   
int nfmts = 0;
string fmts[100];
int varcnt = 0
		       ;
void Parser::geninst(TreeNode *node){
  string fmt = "";
  int nparams = 0;
  const int MAXVARBYTES = 100;
  if (node != NULL) {
    geninst(node->leftChild);
    geninst(node->rightChild);
    switch (node->op) {
    case SEQ:
      {
      break;
      }
    case LOADV:
      {
	emit(";; LOADV " + node->val); 
        string val = node->val;
        const char* c = val.c_str();
        int varnum = atoi(c);
        string str = itos(8*varnum);
        string str2 = "[rbp-" + str + "]";
        emit("push qword" + str2);
      }
      break;
    case LOADL:
      {
	emit(";; LOADL " + node->val);
        emit("mov rax," + node->val);
        emit("push rax");
      }
      break;
    case ADD:
      {
	emit(";; ADD");
        emit("pop rbx");
        emit("pop rax");
        emit("add rax,rbx");
        emit("push rax");
      }
      break;
    case SUB:
      {
	emit(";; SUB");
        emit("pop rbx");
        emit("pop rax");
        emit("sub rax,rbx");
        emit("push rax");
      }
      break;
    case MULT:
      {
	emit(";; MULT");
        emit("pop rbx");
        emit("pop rax");
        emit("imul rbx");
        emit("push rax");
      }
      break;
    case DIV:
      {
	emit(";; DIV");
        emit("mov rdx, 0");
        emit("pop rbx");
        emit("pop rax");
        emit("idiv rbx");
        emit("push rax");
      }
      break;
    case STORE:
      {
	emit(";; STORE " + node->val);
        string val = node->val;
        //converting string val to const char*
        const char* c = val.c_str();
        //converting const char* to integer
        int varnum = atoi(c);
        //multiplying int and converting it back to string
        string str = itos(varnum*8);
        string str2 = "[rbp-" + str + "]";
        emit("pop qword" + str2);
      }
      break;
    case AND:
      {
	emit(";; AND");
        emit("pop rbx");
        emit("pop rax");
        emit("and rax,rbx");
        emit("push rax");
      }
      break;
    case OR:
      {
	emit(";; OR");
        emit("pop rbx");
        emit("pop rax");
        emit("or rax,rbx");
        emit("push rax");
      }
      break;
    case LABEL:
      {
      emit(node->val);
      }
      break;
    case ISEQ:
      {
	emit(";; ISEQ");
        isOps("je");
      }
      break;
    case ISNE:
      {
	emit(";; ISNE");
        isOps("jne");
      }
      break;
    case ISLT:
      {
	emit(";; ISLT");
        isOps("jl");
      }
      break;
    case ISLE:
      {
	emit(";; ISLE");
        isOps("jle");
      }
      break;
    case ISGT:
      {
	emit(";; ISGT");
        isOps("jg");
      }
      break;
    case ISGE:
      {
	emit(";; ISGE");
        isOps("jge");
      }
      break;
    case JUMP:
      {
        emit(";; JUMP " + node->val);
        emit("jmp " + node->val);
      }
      break;
    case JUMPF:
      {
	emit(";; JUMPF " + node->val);
        emit("pop rax");
        emit("cmp rax,0");
        emit("je " + node->val);
      }
      break;
    case JUMPT:
      {
	emit(";; JUMPT " + node->val);
	emit("pop rax");
        emit("cmp rax,0");
        emit("jne " + node->val);
      }
      break;
    case CALL:
      {
	emit(";; CALL " + node->val);
        emit("call " + node->val);
        emit("pop rbx");
        emit("add rsp,rbx");
        emit("push rax");
      }
      break;
    case FUNC:
      {
	emit(";; FUNC " + node->val + ":");
        emit(node->val + ":");
        emit("push rbp");
        emit("mov rbp,rsp");
        emit("sub rsp," + itos(MAXVARBYTES));
        varcnt = 0;
      }
      break;
    case RET:
      {
	emit(";; RET");
        emit("pop rax");
        emit("add rsp," + itos(MAXVARBYTES));
        emit("pop rbp");
        emit("ret");
      }
      break;
    case PRINTF:
      {
	emit(";; PRINTF");
        fmt = node->val;
        nparams = fmt.at(0) - '0';
        fmt = "`" + fmt.substr(1) + "`";
        fmts[nfmts++] = fmt;
        emit(" mov rdi,fmt" + itos(nfmts));
        if (nparams == 5) {
	  emit(" pop r9");
	  --nparams;
        }
        if (nparams == 4) {
	  emit(" pop r8");
	  --nparams;
        }
        if (nparams == 3) {
	  emit(" pop rcx");
	  --nparams;
        }
        if (nparams == 2) {
	  emit(" pop rdx");
	  --nparams;
        }
        if (nparams == 1) {
	  emit(" pop rsi");
        }
        emit(" mov rax,0");
        emit(" push rbp");
        emit(" call printf");
        emit(" pop rbp");
      }
      break;
    case PARAM:
      {
	emit(";; PARAM " + node->val);
        ++varcnt;
        //turn node's value into const char*
        const char * c = node->val.c_str();
        //convert const char* to integer through atoi
        int params = atoi(c);
        //turn integer back to a string after adding & multiplying
        string varc2 = itos((params+2)*8);
        emit("mov rsi, qword[rbp+" + varc2 + "]");
        //multiply varcnt by 8 and turn it to a string
        string varc = itos(varcnt* 8);
        emit("mov qword[rbp-" + varc + "],rsi");
      }
      break;
    default:
      cerr << "In geninst: Unknown operation " << node->op << endl;
      exit(1);
      break;
    }
  }
}

void Parser::genasm(TreeNode *node){
  emit("global main");
  emit("extern printf");
  emit("section text");
  geninst(node);
  cout << endl << " section .data" << endl;
  for (int i=0; i < nfmts; ++i) {
    cout << " fmt" << i+1 << ": db " << fmts[i] <<
      ", 0" << endl;
  }
}   
