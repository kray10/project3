#ifndef LILC_AST_HPP
#define LILC_AST_HPP

#include <ostream>
#include <list>
#include "symbols.hpp"

//Here is a suggestion for all the different kinds of AST nodes
// and what kinds
// of children they have. All of these kinds of AST nodes are
// subclasses of ASTNode.
// Indentation indicates further subclassing:
//
//     Subclass		Children
//     --------		------
//     ProgramNode	DeclListNode
//     DeclListNode	linked list of DeclNode
//     DeclNode
//       VarDeclNode	TypeNode, IdNode, int
//       FnDeclNode	TypeNode, IdNode, FormalsListNode, FnBodyNode
//       FormalDeclNode    TypeNode, IdNode
//       StructDeclNode    IdNode, DeclListNode
//
//     FormalsListNode     linked list of FormalDeclNode
//     FnBodyNode          DeclListNode, StmtListNode
//     StmtListNode        linked list of StmtNode
//     ExpListNode         linked list of ExpNode
//
//     TypeNode:
//       IntNode           -- none --
//       BoolNode          -- none --
//       VoidNode          -- none --
//       StructNode        IdNode
//
//     StmtNode:
//       AssignStmtNode      AssignNode
//       PostIncStmtNode     ExpNode
//       PostDecStmtNode     ExpNode
//       ReadStmtNode        ExpNode
//       WriteStmtNode       ExpNode
//       IfStmtNode          ExpNode, DeclListNode, StmtListNode
//       IfElseStmtNode      ExpNode, DeclListNode, StmtListNode,
//                                    DeclListNode, StmtListNode
//       WhileStmtNode       ExpNode, DeclListNode, StmtListNode
//       CallStmtNode        CallExpNode
//       ReturnStmtNode      ExpNode
//
//     ExpNode:
//       IntLitNode          -- none --
//       StrLitNode          -- none --
//       TrueNode            -- none --
//       FalseNode           -- none --
//       IdNode              -- none --
//       DotAccessNode       ExpNode, IdNode
//       AssignNode          ExpNode, ExpNode
//       CallExpNode         IdNode, ExpListNode
//       UnaryExpNode        ExpNode
//         UnaryMinusNode
//         NotNode
//       BinaryExpNode       ExpNode ExpNode
//         PlusNode
//         MinusNode
//         TimesNode
//         DivideNode
//         AndNode
//         OrNode
//         EqualsNode
//         NotEqualsNode
//         LessNode
//         GreaterNode
//         LessEqNode
//         GreaterEqNode
//
//
// Here are the different kinds of AST nodes again, organized according to
// whether they are leaves, internal nodes with linked lists of kids, or
// internal nodes with a fixed number of kids:
//
// (1) Leaf nodes:
//        IntNode,   BoolNode,  VoidNode,  IntLitNode,  StrLitNode,
//        TrueNode,  FalseNode, IdNode
//
// (2) Internal nodes with (possibly empty) linked lists of children:
//        DeclListNode, FormalsListNode, StmtListNode, ExpListNode
//
// (3) Internal nodes with fixed numbers of kids:
//        ProgramNode,     VarDeclNode,     FnDeclNode,     FormalDeclNode,
//        StructDeclNode,  FnBodyNode,      StructNode,     AssignStmtNode,
//        PostIncStmtNode, PostDecStmtNode, ReadStmtNode,   WriteStmtNode
//        IfStmtNode,      IfElseStmtNode,  WhileStmtNode,  CallStmtNode
//        ReturnStmtNode,  DotAccessNode,   CallExpNode,
//        UnaryExpNode,    BinaryExpNode,   UnaryMinusNode, NotNode,
//        PlusNode,        MinusNode,       TimesNode,      DivideNode,
//        AndNode,         OrNode,          EqualsNode,     NotEqualsNode,
//        LessNode,        GreaterNode,     LessEqNode,     GreaterEqNode
//
// **********************************************************************

// **********************************************************************
// ASTnode class (base class for all other kinds of nodes)
// **********************************************************************

namespace LILC{

class SymSymbol;
class DeclListNode;
class DeclNode;
class TypeNode;
class IdNode;
class FormalsListNode;
class FnBodyNode;
class StmtNode;
class StmtListNode;
class AssignNode;
class ExpNode;
class CallExpNode;

class ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
	void doIndent(std::ostream& out, int indent){
		for (int k = 0 ; k < indent; k++){ out << " "; }
	}
};

class ProgramNode : public ASTNode{
public:
	ProgramNode(DeclListNode * L) : ASTNode(){
		myDeclList = L;
	}
	void unparse(std::ostream& out, int indent);
private:
	DeclListNode * myDeclList;

};

class ExpNode : public ASTNode {
public:
	ExpNode() : ASTNode() {
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class ExpListNode : public ASTNode {
public:
	ExpListNode(std::list<ExpNode *> expList) : ASTNode() {
		myExpList = expList;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<ExpNode *> myExpList;
};

class DeclListNode : public ASTNode{
public:
	DeclListNode(std::list<DeclNode *> * decls) : ASTNode(){
		myDecls = *decls;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<DeclNode *> myDecls;
};

class DeclNode : public ASTNode{
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class VarDeclNode : public DeclNode{
public:
	VarDeclNode(TypeNode * type, IdNode * id, int size) : DeclNode(){
		myType = type;
		myId = id;
		mySize = size;
	}
	void unparse(std::ostream& out, int indent);
	static const int NOT_STRUCT = -1; //Use this value for mySize
					  // if this is not a struct type
private:
	TypeNode * myType;
	IdNode * myId;
	int mySize;
};

class TypeNode : public ASTNode{
public:
	TypeNode() : ASTNode(){
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IdNode : public ExpNode{
public:
	IdNode(IDToken * token) : ExpNode(){
		myStrVal = token->value();
	}
	void unparse(std::ostream& out, int indent);
private:
	std::string myStrVal;
};

class IntNode : public TypeNode{
public:
	IntNode(): TypeNode(){
	}
	void unparse(std::ostream& out, int indent);
};

class BoolNode : public TypeNode {
public:
	BoolNode() : TypeNode() {
	}
	void unparse(std::ostream& out, int indent);
};

class VoidNode : public TypeNode {
public:
	VoidNode() : TypeNode() {
	}
	void unparse(std::ostream& out, int indent);
};

class StructDeclNode : public DeclNode {
public:
	StructDeclNode(IdNode * id, DeclListNode * declList) : DeclNode() {
		myId = id;
		myDeclList = declList;
	}
	void unparse(std::ostream& out, int indent);
private:
	IdNode * myId;
	DeclListNode * myDeclList;
};

class StructNode : public TypeNode {
public:
	StructNode(IdNode * id) : TypeNode() {
		myId = id;
	}
	void unparse(std::ostream& out, int indent);
private:
	IdNode * myId;
};

class FnDeclNode : public DeclNode {
public:
	FnDeclNode(TypeNode * type, IdNode * id, FormalsListNode * formalsList,
		FnBodyNode * fnBody) : DeclNode() {
			myType = type;
			myId = id;
			myFormalsList = formalsList;
			myFnBody = fnBody;
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IdNode * myId;
	FormalsListNode * myFormalsList;
	FnBodyNode * myFnBody;
};

class FormalDeclNode : public DeclNode {
public:
	FormalDeclNode(TypeNode * type, IdNode * id) : DeclNode() {
		myType = type;
		myId = id;
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IdNode * myId;
};

class FormalsListNode : public ASTNode {
public:
	FormalsListNode(std::list<FormalDeclNode *> * formalDeclList) : ASTNode() {
		myFormalDeclList = *formalDeclList;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<FormalDeclNode *> myFormalDeclList;
};

class FnBodyNode : public ASTNode {
public:
	FnBodyNode(DeclListNode * declList, StmtListNode * stmtList) : ASTNode() {
		myDeclList = declList;
		myStmtList = stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	DeclListNode * myDeclList;
	StmtListNode * myStmtList;
};

class StmtListNode : public ASTNode {
public:
	StmtListNode(std::list<StmtNode *> * stmtList) : ASTNode() {
		myStmtList = *stmtList;
	}
	void unparse(std::ostream& out, int indent);
private:
	std::list<StmtNode *> myStmtList;
};

class StmtNode : public ASTNode {
public:
	StmtNode() : ASTNode() {
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class AssignStmtNode : public StmtNode {
public:
	AssignStmtNode(AssignNode * assignNode) : StmtNode() {
		myAssignNode = assignNode;
	}
	void unparse(std::ostream& out, int indent);
private:
	AssignNode * myAssignNode;
};

class AssignNode : public ExpNode {
public:
	AssignNode(ExpNode * expNode1, ExpNode * expNode2) : ExpNode() {
		myExpNode1 = expNode1;
		myExpNode2 = expNode2;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myExpNode1;
	ExpNode * myExpNode2;
};

class DotAccessNode : public ExpNode {
public:
	DotAccessNode(ExpNode * expNode, IdNode * id) : ExpNode() {
		myExp = expNode;
		myId = id;
	}
	void unparse(std::ostream& out, int indent);
private:
	ExpNode * myExp;
	IdNode * myId;
};

class IntLitNode : public ExpNode {
public:
	IntLitNode(IntLitToken * intLit) : ExpNode() {
		myIntLit = intLit;
	}
	void unparse(std::ostream& out, int indent);
private:
	IntLitToken * myIntLit;
};

class StrLitNode : public ExpNode {
public:
	StrLitNode(StringLitToken * stringLit) : ExpNode() {
		myStringLit = stringLit;
	}
	void unparse(std::ostream& out, int indent);
private:
	StringLitToken * myStringLit;
};

class TrueNode : public ExpNode {
public:
	TrueNode() : ExpNode() {
	}
	void unparse(std::ostream& out, int indent);
};

} //End namespace LIL' C

#endif
