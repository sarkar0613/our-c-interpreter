# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string.h>
# include <vector>
# include <cstdlib>
# include <stack>
# include <string>
using namespace std ;
static int uTestnum = 0 ;

struct Token {
  string tokenType ;
  string tokenName ;
  float value ;
  int row ;
  int column ;
} ;

struct Function {
  string functionName ;
  vector<string> content ;  
};

struct Variable {
  string type ;  
  string name ;
  string index ;
  int array ;  
};

int grow = 0 ; 
int gcolumn = 0 ;
int gchoice =  0 ;
int gisprint = 0 ;
int gisFunction = 0 ;
int gcount = 0 ;         // 算{}數 
int grepeat = 0 ;        // 看func命名有無重複  
int grepeat1 = 0 ;      // 看variable命名有無重複 
int gCompound = 0 ;     // 是否進{  } 
string gTempFunc = "" ;    // 暫存funcName 
vector<Token> gtokenList ; // 存所有讀進來的token 
vector<Token> gback ; // 退還多讀的token 
vector<Token> gtoken ; // 判斷那行是不是特定的function 
vector<Token> gIdent ; // 這行新定義的ident 
vector<Token> gIdent1 ; // 已經定義的ident 
vector<Token> gFunction ; // 這次定義的function 
vector<Token> gFunction1 ; // Function裡定義的ident
vector<Token> gFunction2 ; // 已經定義的function 
vector<Function> gFunTable ; // 紀錄每個function的內容 
vector<Variable> gVariTable ;

Token gGet ;
Variable gTemp ;

void CutToken() ;
void ReadTrash( char ch ) ;
void ReadAnnotation( char ch, char next ) ;
void TokenDigital( char ch ) ;
void TokenLetter( char ch ) ;
void TokenSymbol( char ch ) ;
void TokenDoubleQuotes( char ch ) ;
void TokenSingleQuotes( char ch ) ;
// gettoken
bool SymbolDetermine( char ch, char next ) ;
void TypeDetermine( string tName, string &tType, float &tValue ) ;
bool IsDigital( char a ) ;
bool IsLetter( char a ) ;
bool IsSymbol( char a ) ;
void Error1( char ch ) ;
void Error2( string a ) ;
void Error3( string a ) ;
void SaveToken( string tType, string tName, float tValue, int row, int column ) ;
void SaveTokenList( string tType, string tName, float tValue, int row, int column ) ;
void SaveIdent( Token gGet ) ;
void SaveIdent1() ;
void SaveFunction() ;
void SaveFuncToFunc2() ;
void SaveFunctionCont() ;
void PushToFunction1( Token gGet ) ;
// tools
void User_input( Token token ) ;
void Definition( Token token ) ;
void Function_definition_or_declarators( Token token ) ;
void Rest_of_declarators( Token token ) ;
void Function_definition_without_ID( Token token ) ;
void Formal_parameter_list( Token token ) ;
void Compound_statement( Token token ) ;
void Declaration( Token token ) ;
void Statement( Token token ) ;
void Expression( Token token ) ;
void Basic_expression( Token token ) ;
void Rest_of_Identifier_started_basic_exp( Token token ) ;
void Rest_of_PPMM_Identifier_started_basic_exp( Token token ) ;
void Actual_parameter_list( Token token ) ;
void Romce_and_romloe( Token token ) ;
void Rest_of_maybe_logical_OR_exp( Token token ) ;
void Maybe_logical_AND_exp( Token token ) ;
void Rest_of_maybe_logical_AND_exp( Token token ) ;
void Maybe_bit_OR_exp( Token token ) ;
void Rest_of_maybe_bit_OR_exp( Token token ) ;
void Maybe_bit_ex_OR_exp( Token token ) ;
void Rest_of_maybe_bit_ex_OR_exp( Token token ) ;
void Maybe_bit_AND_exp( Token token ) ;
void Rest_of_maybe_bit_AND_exp( Token token ) ;
void Maybe_equality_exp( Token token ) ;
void Rest_of_maybe_equality_exp( Token token ) ;
void Maybe_relational_exp( Token token ) ;
void Rest_of_maybe_relational_exp( Token token ) ;
void Maybe_shift_exp( Token token ) ;
void Rest_of_maybe_shift_exp( Token token ) ;
void Maybe_additive_exp( Token token ) ;
void Rest_of_maybe_additive_exp( Token token ) ;
void Maybe_mult_exp( Token token ) ;
void Rest_of_maybe_mult_exp( Token token ) ;
void Unary_exp( Token token ) ;
void Signed_unary_exp( Token token ) ;
void Unsigned_unary_exp( Token token ) ;
void CleargTemp() ;
bool IsType_specifier( Token token ) ;
bool IsRest_of_declarators( Token token ) ;
bool IsSign( Token token ) ;
bool Done() ;
bool ListAllFunctions() ;
bool ListFunctions() ;
bool ListAllVariables() ;
bool ListVariable() ;
void SortFunc2() ;
void PrintFunctionCont() ;
void PrintAllVariable() ;
void PrintVariable() ;
void PrintDefinition( string tokenID, int num ) ;
void PrintFunction( string name ) ;
bool HasDefined( string name ) ;
bool HasDefined1( string name ) ;
bool HasDefined2( string name ) ;
void ReadError() ;
bool HasFunc( string name, int &num ) ;
// grammer 
void StartMSG() ;
void Test() ;

bool IsType_specifier( Token token ) {
    
  if ( token.tokenType == "int" || token.tokenType == "char" || token.tokenType == "float" || 
       token.tokenType == "string" || token.tokenType == "bool" )
    return true ;
  else
    return false ;    
      
} // IsType_specifier()

bool IsRest_of_declarators( Token token ) {
    
  if ( token.tokenType == "[" || token.tokenType == "," || token.tokenType == ";" )
    return true ;
  else
    return false ;    
      
} // IsRest_of_declarators()

bool IsSign( Token token ) {
    
  if ( token.tokenType == "+" || token.tokenType == "-" || token.tokenType == "!" )
    return true ;
  else
    return false ;  
    
} // IsSign()

void BackToken( Token token ) {
  Token back ; 
  back.column = token.column ;
  back.row = token.row ;
  back.tokenName = token.tokenName ;
  back.tokenType = token.tokenType ;
  back.value = token.value ;
  gback.push_back( token ) ;  
    
} // BackToken()

void PushToFunction1( Token gGet ) {
  Token token ;  
  token.tokenType = gGet.tokenType ;
  token.tokenName = gGet.tokenName ;
  token.value = gGet.value ;
  token.row = gGet.row ;
  token.column = gGet.column ;
  gFunction1.push_back( token ) ;
    
} // PushToFunction1()

bool Done() { 
  if ( gtoken[0].tokenName == "Done" && gtoken[1].tokenName == "(" && gtoken[2].tokenName == ")" &&
       gtoken[3].tokenName == ";" ) 
    return true ;
  else
    return false ;
           
} // Done()

bool ListAllFunctions() {
    
  if ( gtoken[0].tokenName == "ListAllFunctions" && gtoken[1].tokenName == "(" && 
       gtoken[2].tokenName == ")" && gtoken[3].tokenName == ";" ) 
    return true ;
  else 
    return false ;
           
} // ListAllFunctions()

bool ListFunction() {  
  if ( gtoken[0].tokenName == "ListFunction" && gtoken[1].tokenName == "(" && 
       gtoken[2].tokenType == "const" && gtoken[3].tokenName == ")" && gtoken[4].tokenName == ";" ) 
    return true ;
  else 
    return false ;
  
} // ListFunction()

bool ListAllVariables() {  
  if ( gtoken[0].tokenName == "ListAllVariables" && gtoken[1].tokenName == "(" && 
       gtoken[2].tokenName == ")" && gtoken[3].tokenName == ";" ) 
    return true ;
  else 
    return false ;
  
} // ListAllVariables()

bool ListVariable() {  
  if ( gtoken[0].tokenName == "ListVariable" && gtoken[1].tokenName == "(" && 
       gtoken[2].tokenType == "const" && gtoken[3].tokenName == ")" && gtoken[4].tokenName == ";" ) 
    return true ;
  else 
    return false ;
  
} // ListVariable()

void PrintDefinition( string tokenID, int num ) {  
  int reName = 0 ;
  
  for ( int i = 0 ; i < gIdent1.size() ; i ++ ) {
    if ( gIdent1[i].tokenName == tokenID ) {
      reName = 1 ; 
      gIdent.erase( gIdent.begin() + num ) ;
    } // if   
  } // for
  
  if ( reName == 1 ) {
    cout << "New definition of " << tokenID << " entered ..." << endl ;
    grepeat1 = 1 ;    
  } // if
  else {  
    cout << "Definition of " << tokenID << " entered ..." << endl ;
  } // else 
  
  gisprint = 1 ;  
} // PrintDefinition()

bool HasDefined( string name ) {
  int a = 0 ;
  for ( int i = 0 ; i < gIdent1.size() ; i ++ ) {
    if ( gIdent1[i].tokenName == name )
      a = 1 ;  
  } // for
  
  for ( int j = 0 ; j < gFunction2.size() ; j ++ ) {
    if ( gFunction2[j].tokenName == name )  
      a = 1 ;
  } // for
  
  if ( a == 1 )
    return true ;
  else
    return false ;
       
} // HasDefined()

bool HasDefined2( string name ) {
  int a = 0 ;
  for ( int k = 0 ; k < gIdent.size() ; k ++ ) {
    if ( gIdent[k].tokenName == name )
      a = 1 ;  
  } // for
    
  for ( int i = 0 ; i < gIdent1.size() ; i ++ ) {
    if ( gIdent1[i].tokenName == name )
      a = 1 ;  
  } // for
  
  for ( int j = 0 ; j < gFunction2.size() ; j ++ ) {
    if ( gFunction2[j].tokenName == name )  
      a = 1 ;
  } // for

  if ( a == 1 )
    return true ;
  else
    return false ;
       
} // HasDefined2()

void User_input() {

  CutToken() ;
  grow = 1 ;
  while ( gGet.tokenName != "done" ) {  
    try { 
      if ( IsType_specifier( gGet ) || gGet.tokenType == "void" ) 
        Definition( gGet ) ;
      else 
        Statement( gGet ) ; 
      
      SaveFunctionCont() ;
      if ( gCompound == 0 )
        SaveIdent1() ;
      if ( grepeat == 0 )
        SaveFuncToFunc2() ;

      if ( Done() ) {
        cout << "> Our-C exited ..." << endl ;
        gIdent1.clear() ;
        gIdent.clear() ;
        return ;
      } // if
      else if ( ListAllFunctions() ) 
        SortFunc2() ; 
      else if ( ListFunction() ) 
        PrintFunctionCont() ;  
      else if ( ListAllVariables() ) 
        PrintAllVariable() ;  
      else if ( ListVariable() ) 
        PrintVariable() ;  
      else if ( gisprint == 0 ) 
        cout << "> Statement executed ..." << endl ; 
      grow = 0 ;
    } // try
    
    catch ( string error ) {   
      
      if ( gchoice == 1 ) {
        cout << "> Line " << grow << " : unrecognized token with first char : '" ; 
        cout << error << "'" << endl ;    
      } // if
      else if ( gchoice == 2 ) {
        cout << "> Line " << grow << " : unexpected token : '" << error << "'" << endl ;     
      } // else if
      else if ( gchoice == 3 ) {
        cout << "> Line " << grow << " : undefined identifier : '" << error << "'" << endl ;   
      } // else if
      
      ReadError() ;

    } // catch
    gCompound = 0 ; 
    gisprint = 0 ;
    gisFunction = 0 ;
    gcount = 0 ;
    grepeat = 0 ;
    grepeat1 = 0 ;
    gFunction.clear() ;
    gFunction1.clear() ;
    gIdent.clear() ;
    gtoken.clear() ;
    CleargTemp() ;

    CutToken() ;

  } // while
  
} // User_input()

void PrintAllVariable() {

  for ( int i = 0 ; i < gIdent1.size() - 1 ; i++ ) {
    for ( int j = 0 ; j < gIdent1.size() - i - 1 ; j++ ) {
      if ( gIdent1[j].tokenName.compare( gIdent1[j+1].tokenName ) > 0 ) {
        Token temp;
        temp.tokenName = gIdent1[j].tokenName ;
        temp.tokenType = gIdent1[j].tokenType ;
        gIdent1[j].tokenName = gIdent1[j + 1].tokenName ;
        gIdent1[j].tokenType = gIdent1[j + 1].tokenType ;
        gIdent1[j + 1].tokenName = temp.tokenName ;
        gIdent1[j + 1].tokenType = temp.tokenType ;
  
      } // if
    } // for
  }  // for    
  
  cout << "> " ;
  for ( int i = 0 ; i < gIdent1.size() ; i ++ ) {
    cout << gIdent1[i].tokenName << endl ;
  } // for 
  
  cout << "Statement executed ..." << endl ;     
    
} // PrintAllVariable()

void PrintVariable() {
  string name ;
  int a = 0 ;
  int num = 0 ;
  for ( int j = 1 ; j < gtoken[2].tokenName.size() - 1 ; j ++ ) {
    name = name + gtoken[2].tokenName[j] ;
  } // for
  
  for ( int i = 0 ; i < gVariTable.size() ; i ++ ) {
    if ( gVariTable[i].name == name ) {
      a = 1 ;
      num = i ;
    } // if
  } // for
  
  if ( a == 1 ) {
    cout << "> " << gVariTable[num].type << " " << gVariTable[num].name ;
    if ( gVariTable[num].array == 1 )
      cout << "[ " << gVariTable[num].index << " ]" << " ;" << endl ;
    else
      cout << " ;" << endl ;  
     
  } // if
  else
    Error3( name ) ; 
  cout << "Statement executed ..." << endl ;

} // PrintVariable() 

void PrintFunctionCont() {
  
  string name ;
  int num = 0 ;
  int space = 0 ;
  bool first = true ;
  for ( int i = 1 ; i < gtoken[2].tokenName.size() - 1 ; i ++ ) {
    name = name + gtoken[2].tokenName[i] ;
  } // for

  if ( HasFunc( name, num ) ) {
    cout << "> " ;
    for ( int j = 0 ; j < gFunTable[num].content.size() ; j ++ ) {
      if ( first ) {  
        if ( gFunTable[num].content[j] == ";" || gFunTable[num].content[j] == "{" ) {
          cout << " " << gFunTable[num].content[j] << endl ;
          first = false ;
        } // if
        else if ( gFunTable[num].content[j] == "(" ) {
          for ( int k = 0 ; k < space ; k ++ )
            cout << " " ;   
          cout << gFunTable[num].content[j] ; 
          first = false ; 
        } // else if
        else if ( gFunTable[num].content[j] == "}" ) { 
          space = space - 2 ;  
          for ( int k = 0 ; k < space ; k ++ )
            cout << " " ;  
          cout << gFunTable[num].content[j] << endl ; 
          first = true ; 
        } // else if
        else if ( gFunTable[num].content[j] == "while" || gFunTable[num].content[j] == "if" ) {
          for ( int k = 0 ; k < space ; k ++ )
            cout << " " ;
          cout << gFunTable[num].content[j] << " " ;
          first = false ;  
        } // else if
        else {
          for ( int k = 0 ; k < space ; k ++ )
            cout << " " ;
          cout << gFunTable[num].content[j] ;
          first = false ;  
        } // else 
        
      } // if
      else {
        if ( gFunTable[num].content[j] == "{" ) {
          cout << " " << gFunTable[num].content[j] << endl ;
          space = space + 2 ;
          first = true ;
        } // if
        else if ( gFunTable[num].content[j] == ";" ) {
          cout << " " << gFunTable[num].content[j] << endl ;
          first = true ;
        } // else if 
        else if ( gFunTable[num].content[j] == "(" ) {
          cout << gFunTable[num].content[j] ;  
        } // else if
        else if ( gFunTable[num].content[j] == "}" ) {
        } // else if
        else if ( gFunTable[num].content[j] == "[" || gFunTable[num].content[j] == "++" || 
                  gFunTable[num].content[j] == "--" ) { 
          cout << gFunTable[num].content[j] ;  
        } // else if
        else { 
          cout << " " << gFunTable[num].content[j] ;
        } // else
      } // else
        
    } // for 
      
  } // if
  else
    Error3( name ) ; 
  cout << "Statement executed ..." << endl ; 
    
} // PrintFunctionCont() 

bool HasFunc( string name, int &num ) {
  int a = 0 ;
  for ( int i = 0 ; i < gFunTable.size() ; i ++ ) {
    if ( gFunTable[i].functionName == name ) {
      num = i ;  
      a = 1 ;
    } // if
  } // for
  
  if ( a == 1 )
    return true ;
  else
    return false ;
       
} // HasFunc()

void ReadError() {
  char next = cin.peek() ;
  char ch = '\0' ;
  while ( next != '\n' ) {
    ch = getchar() ;  
    next = cin.peek() ;  
  } // while 
    
  ch = getchar() ;
  grow = 1 ;
} // ReadError()

void SortFunc2() {
  for ( int i = 0 ; i < gFunction2.size() - 1 ; i++ ) {
    for ( int j = 0 ; j < gFunction2.size() - i - 1 ; j++ ) {
      if ( gFunction2[j].tokenName.compare( gFunction2[j+1].tokenName ) > 0 ) {
        Token temp;
        temp.tokenName = gFunction2[j].tokenName ;
        temp.tokenType = gFunction2[j].tokenType ;
        gFunction2[j].tokenName = gFunction2[j + 1].tokenName ;
        gFunction2[j].tokenType = gFunction2[j + 1].tokenType ;
        gFunction2[j + 1].tokenName = temp.tokenName ;
        gFunction2[j + 1].tokenType = temp.tokenType ;
      } // if
    } // for
  }  // for 
     
  cout << "> " ;
  for ( int i = 0 ; i < gFunction2.size() ; i ++ ) {
    cout << gFunction2[i].tokenName << "()" << endl ;
  } // for 
  
  cout << "Statement executed ..." << endl ;  
} // SortFunc2()

void SaveFunctionCont() {
  if ( gisFunction == 1 ) {
    Function temp ;
    string temp1 ;
    vector<string> temp2 ;
    
    for ( int i = 0 ; i < gtoken.size() ; i ++ ) {
      temp1 = gtoken[i].tokenName ;
      temp2.push_back( temp1 ) ;
    } // for  
    
    for ( int j = 0 ; j < temp2.size() ; j ++ ) {
      temp.content.push_back( temp2[j] ) ;   
    } // for
    
    temp.functionName = gTempFunc ;  
    gFunTable.push_back( temp ) ;  
  } // if 
  
} // SaveFunctionCont()

void Definition( Token token ) {
    
  if ( gGet.tokenType == "void" ) {
    gisFunction = 1 ;  
    CutToken() ;    
    if ( gGet.tokenType == "ident" ) {
      gTempFunc = gGet.tokenName ;
      SaveIdent( gGet ) ;
      SaveFunction();  
      CutToken() ;
      Function_definition_without_ID( gGet ) ;
    } // if
    else 
      Error2( gGet.tokenName ) ;        
  } // if
    
  else { 
    gTemp.type = gGet.tokenName ;
    CutToken() ;
    if ( gGet.tokenType == "ident" ) {
      gTemp.name = gGet.tokenName ;
      SaveIdent( gGet ) ;
      gTempFunc = gGet.tokenName ;
      CutToken() ;
      Function_definition_or_declarators( gGet ) ;
    } // if
    else
      Error2( gGet.tokenName ) ;   
           
  } // else 
    
} // Definition()

void CleargTemp() {
  gTemp.array = 0 ;
  gTemp.index = "" ;
  gTemp.name = "" ;
  gTemp.type = "" ;  
} // CleargTemp()

void SaveFunction() {
  Token token ;  
  token.tokenType = gIdent[ gIdent.size()-1 ].tokenType ;
  token.tokenName = gIdent[ gIdent.size()-1 ].tokenName ;
  token.value = gIdent[ gIdent.size()-1 ].value ;
  token.row = gIdent[ gIdent.size()-1 ].row ;
  token.column = gIdent[ gIdent.size()-1 ].column ; 
  gFunction.push_back( token ) ; 
  gIdent.pop_back() ;   
} // SaveFunction()

void Function_definition_or_declarators( Token token ) {
 
  if ( gGet.tokenType == "(" ) {
    SaveFunction();
    gisFunction = 1 ;
    CleargTemp() ;
    Function_definition_without_ID( gGet ) ;
  } // if
  else if ( IsRest_of_declarators( gGet ) ) {
    gTempFunc = "" ;
    Rest_of_declarators( token ) ;
  } // else if
  else {
    Error2( gGet.tokenName ) ;
  } // else 
    
} // Function_definition_or_declarators()

void Rest_of_declarators( Token token ) {   
  string type1 = "" ;
  type1 = gTemp.type ;
  if ( gGet.tokenType == "[" ) {
    gTemp.array = 1 ;  
    CutToken() ;
    if ( gGet.tokenType == "const" )
      gTemp.index = gGet.tokenName ;
    else   
      Error2( gGet.tokenName ) ;
    CutToken() ;
    if ( gGet.tokenType != "]" ) 
      Error2( gGet.tokenName ) ;  
    CutToken() ;
  }   // if
  
  gVariTable.push_back( gTemp ) ;
  
  while ( gGet.tokenType == "," ) { 
    CleargTemp() ; 
    gTemp.type = type1 ;
    CutToken() ;
    if ( gGet.tokenType == "ident" )
      gTemp.name = gGet.tokenName ;
    else   
      Error2( gGet.tokenName ) ;
    SaveIdent( gGet ) ; 
    CutToken() ;
    if ( gGet.tokenType == "[" ) {
      gTemp.array = 1 ;  
      CutToken() ;
      if ( gGet.tokenType == "Constant" )
        gTemp.index = gGet.tokenName ;
      else   
        Error2( gGet.tokenName ) ;
        
      CutToken() ;
      if ( gGet.tokenType != "]" ) 
        Error2( gGet.tokenName ) ;
        
      CutToken() ;
      
    }   // if  
    
    gVariTable.push_back( gTemp ) ;
  } // while
  
  if ( gGet.tokenType != ";" )   
    Error2( gGet.tokenName ) ;
  if ( gisFunction == 0 && gCompound == 0 ) {
    cout << "> " ;  
    for ( int i = 0 ; i < gIdent.size() ; i ++ ) {
      PrintDefinition( gIdent[i].tokenName, i ) ;
    } // for
  } // if
  
} // Rest_of_declarators()

void Function_definition_without_ID( Token token ) {
      
  if ( gGet.tokenType == "(" ) {
    CutToken() ;
    if ( gGet.tokenType == "void" ) {
      CutToken() ;
      if ( gGet.tokenType == ")" ) {
        CutToken() ;
        Compound_statement( gGet ) ;
      } // if
      else {
        Error2( gGet.tokenName ) ;
      } // else
          
    } // if
    
    else if ( IsType_specifier( gGet ) ) {
      CutToken() ;
      Formal_parameter_list( gGet ) ;
      if ( gGet.tokenType == ")" ) {
        CutToken() ;
        Compound_statement( gGet ) ;
      } // if
      else
        Error2( gGet.tokenName ) ;
      
    } // else if
    
    else if ( gGet.tokenType == ")" ) {
      CutToken() ;
      Compound_statement( gGet ) ;
    } // else if
    
    else 
      Error2( gGet.tokenName ) ;
      
  } // if
  
  else {
    Error2( gGet.tokenName ) ;
  } // else 
    
} // Function_definition_without_ID() 

void Formal_parameter_list( Token token ) {
 
  if ( gGet.tokenType == "&" ) {
    CutToken() ;
    if ( gGet.tokenType == "ident" ) {
      PushToFunction1( gGet ) ;
      CutToken() ;
    } // if
    else  
      Error2( gGet.tokenName ) ;
        
    if ( gGet.tokenType == "[" ) {
      CutToken() ;
      if ( gGet.tokenType == "const" )
        CutToken() ;
      else 
        Error2( gGet.tokenName ) ;
          
      if ( gGet.tokenType == "]" )
        CutToken() ;  
      else 
        Error2( gGet.tokenName ) ;
    } // if
          
  } // if
          
  else if ( gGet.tokenType == "ident" ) {
    PushToFunction1( gGet ) ; 
    CutToken() ;
    if ( gGet.tokenType == "[" ) {
      CutToken() ;
      if ( gGet.tokenType == "const" )
        CutToken() ;
      else 
        Error2( gGet.tokenName ) ;
          
      if ( gGet.tokenType == "]" )
        CutToken() ;  
      else 
        Error2( gGet.tokenName ) ;
    } // if
           
  } // else if
  else 
    Error2( gGet.tokenName ) ;
    
  while ( gGet.tokenType == "," ) {
    CutToken() ;
    if ( IsType_specifier( gGet ) )
      CutToken() ; 
    else
      Error2( gGet.tokenName ) ;
        
    if ( token.tokenType == "&" ) 
      CutToken() ;
      
    if ( gGet.tokenType == "ident" ) {
      PushToFunction1( gGet ) ;
      CutToken() ;
    } // if
    else  
      Error2( gGet.tokenName ) ;
        
    if ( gGet.tokenType == "[" ) {
      CutToken() ;
      if ( gGet.tokenType == "const" )
        CutToken() ;
      else 
        Error2( gGet.tokenName ) ;
          
      if ( gGet.tokenType == "]" )
        CutToken() ;  
      else 
        Error2( gGet.tokenName ) ;
    } // if
        
  } // while
  
} // Formal_parameter_list()

void PrintFunction( string name ) {
  int reName = 0 ;
  for ( int i = 0 ; i < gFunction2.size() ; i ++ ) {
    if ( gFunction2[i].tokenName == name ) {
      reName = 1 ; 
    } // if   
  } // for
  
  if ( reName == 1 ) {
    cout << "> New definition of " << name << "() entered ..." << endl ;    
    grepeat = 1 ;
  } // if
  else {  
    cout << "> Definition of " << name << "() entered ..." << endl ;
  } // else 
  
  gisprint = 1 ;      
    
} // PrintFunction()

void Compound_statement( Token token ) {
  gCompound = 1 ;  
  if ( token.tokenType == "{" ) {
    gcount ++ ;
    CutToken() ;
    while ( gGet.tokenType != "}" ) {  
      if ( IsType_specifier( gGet ) ) {
        CutToken() ;
        Declaration( gGet ) ;
      } // if
      else {
        Statement( gGet ) ;      
      } // else
      
      CutToken() ;
      if ( gGet.tokenType == "}" )
        gcount -- ;
    } // while
  } // if
  else
    Error2( gGet.tokenName ) ;
    
  if ( gcount == 0 && gisFunction == 1 ) 
    PrintFunction( gFunction[0].tokenName ) ;
  
} // Compound_statement()

void Declaration( Token token ) {
   
  if ( gGet.tokenType == "ident" ) {
    if ( gisFunction == 1 )  
      PushToFunction1( gGet ) ;
    else 
      SaveIdent( gGet ) ; 

    CutToken() ;
    if ( IsRest_of_declarators( gGet ) ) 
      Rest_of_declarators( gGet ) ;
    else 
      Error2( gGet.tokenName ) ;
  } // if
  else 
    Error2( gGet.tokenName ) ;
    
} // Declaration()

void Statement( Token token ) {
    
  if ( gGet.tokenType == ";" )
    return ;
    
  else if ( gGet.tokenType == "return" ) {
    CutToken() ;
    if ( gGet.tokenType == ";" )
      return ;
    else {
      Expression( gGet ) ;
      if ( gGet.tokenType != ";" ) 
        Error2( gGet.tokenName ) ;
    } // else
  } // else if
  
  else if ( gGet.tokenType == "{" ) {
    Compound_statement( token ) ;
  }  // else if
  
  else if ( gGet.tokenType == "if" ) {
    CutToken() ;
    if ( gGet.tokenType == "(" ) {
      CutToken() ;
      Expression( gGet ) ;
          
      if ( gGet.tokenType != ")" ) 
        Error2( gGet.tokenName ) ;
      CutToken() ;   
      Statement( gGet ) ;
      CutToken() ;
      if ( gGet.tokenType == "else" ) {
        CutToken() ;
        Statement( gGet ) ;
      } // if
      else {
        BackToken( gGet ) ; 
      } // else 
      
    } // if
    else 
      Error2( gGet.tokenName ) ; 
      
  } // else if
  
  else if ( gGet.tokenType == "while" ) {
    CutToken() ;
    if ( gGet.tokenType == "(" ) {
      CutToken() ;
      Expression( gGet ) ;
      if ( gGet.tokenType != ")" ) 
        Error2( gGet.tokenName ) ;
      CutToken() ;   
      Statement( gGet ) ;
      
    } // if 
    else
      Error2( gGet.tokenName ) ; 
      
  } // else if
  
  else if ( gGet.tokenType == "do" ) {
    CutToken() ;
    Statement( gGet ) ;
    CutToken() ;
    if ( gGet.tokenType != "while" ) 
      Error2( gGet.tokenName ) ;  
    CutToken() ; 
    if ( gGet.tokenType != "(" ) 
      Error2( gGet.tokenName ) ; 
    CutToken() ;
    Expression( gGet ) ;
    
    if ( gGet.tokenType != ")" ) 
      Error2( gGet.tokenName ) ;
    CutToken() ;  
    if ( gGet.tokenType != ";" ) 
      Error2( gGet.tokenName ) ;  
  } // else if
  else {
    Expression( token ) ;
    if ( gGet.tokenType != ";" ) 
      Error2( gGet.tokenName ) ;     
  } // else
   
} // Statement()

void Expression( Token token ) {
    
  Basic_expression( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "," ) {
    CutToken() ;
    Basic_expression( gGet ) ;  
    CutToken() ;  
  } // while  
    
} // Expression()

bool IsInstruction( string name ) {
  if ( name == "cin" || name == "cout" ) {
    return true ; 
  } // if  
  else if ( name == "Done" || name == "ListAllVariables" || name == "ListAllFunctions" ||
            name == "ListFunction" || name == "ListVariable" ) {
    return true ;  
  } // else if
  else
    return false ;
    
} // IsInstruction()

bool HasDefined1( string name ) {
  int a = 0 ;
  for ( int i = 0 ; i < gFunction1.size() ; i ++ ) {
    if ( gFunction1[i].tokenName == name )
      a = 1 ;  
  } // for
  
  for ( int j = 0 ; j < gFunction.size() ; j ++ ) {
    if ( gFunction[j].tokenName == name )
      a = 1 ;  
  } // for
  
  for ( int k = 0 ; k < gFunction2.size() ; k ++ ) {
    if ( gFunction2[k].tokenName == name )
      a = 1 ;  
  } // for
  
  if ( a == 1 )
    return true ;
  else
    return false ;    
    
} // HasDefined1()

void Basic_expression( Token token ) {
   
  if ( gGet.tokenType == "ident" ) {
    if ( IsInstruction( gGet.tokenName ) ) {      // cin cout 特定function
    } // if
    else if ( gisFunction == 1 ) {                  // 定義function 
      if ( !HasDefined1( gGet.tokenName ) )  
        Error3( gGet.tokenName ) ; 
    } // else if
    else if ( gCompound == 1 ) { 
      if ( !HasDefined2( gGet.tokenName ) )
        Error3( gGet.tokenName ) ; 
    } // else if
    else if ( !HasDefined( gGet.tokenName ) ) {   // 一般定義  
      Error3( gGet.tokenName ) ; 
    } // if
    
    CutToken() ;
    Rest_of_Identifier_started_basic_exp( gGet ) ;   
  } // if
  else if ( gGet.tokenType == "++" || gGet.tokenType == "--" ) {
    CutToken() ;  
    if ( gGet.tokenType == "ident" ) {
      if ( IsInstruction( gGet.tokenName ) ) {      // cin cout 特定function
      } // if
      else if ( gisFunction == 1 ) {                  // 定義function 
        if ( !HasDefined1( gGet.tokenName ) )  
          Error3( gGet.tokenName ) ; 
      } // else if 
      else if ( !HasDefined( gGet.tokenName ) ) {   // 一般定義 
        Error3( gGet.tokenName ) ; 
      } // else if
      
      CutToken() ;  
      Rest_of_PPMM_Identifier_started_basic_exp( gGet ) ; 
    } // if 
    else 
      Error2( gGet.tokenName ) ;
         
  } // else if
  
  else if ( IsSign( gGet ) ) {
    CutToken() ;
    while ( IsSign( gGet )  ) {
      CutToken() ; 
    } // while
    
    Signed_unary_exp( gGet ) ;
    CutToken() ;
    Romce_and_romloe( gGet ) ;
      
  } // else if
  
  else if ( gGet.tokenType == "const" || gGet.tokenType == "(" ) {
    if ( gGet.tokenType == "const" ) {
      CutToken() ;  
      Romce_and_romloe( gGet ) ;  
    } // if
    else if ( gGet.tokenType == "(" ) {
      CutToken() ;  
      Expression( gGet ) ; 
      if ( gGet.tokenType != ")" )
        Error2( gGet.tokenName ) ;
      CutToken() ;  
      Romce_and_romloe( gGet ) ;    
    } // else if
      
  } // else if
  else
    Error2( gGet.tokenName ) ;
    
} // Basic_expression() ;

void Rest_of_Identifier_started_basic_exp( Token token ) {

  if ( gGet.tokenType == "(" ) {
    CutToken() ;
    if ( gGet.tokenType == ")" ) {
      CutToken() ;
      Romce_and_romloe( gGet ) ;
    } // if
    else {
      Actual_parameter_list( gGet ) ;
      if ( gGet.tokenType != ")" )
        Error2( gGet.tokenName ) ;
      CutToken() ;
      Romce_and_romloe( gGet ) ;  
    } // else
  } // if
  
  else {
    if ( gGet.tokenType == "[" ) {
      CutToken() ;
      Expression( gGet ) ; 
      if ( gGet.tokenType != "]" ) 
        Error2( gGet.tokenName ) ;
      CutToken() ;  
    }  // if
      
    if ( gGet.tokenType == "=" || gGet.tokenType == "+=" || gGet.tokenType == "-=" || 
         gGet.tokenType == "*=" || gGet.tokenType == "/=" || gGet.tokenType == "%=" ) {
      CutToken() ;
      Basic_expression( gGet ); 
    } // if
    else if ( gGet.tokenType == "++" || gGet.tokenType == "--" ) {
      CutToken() ;
      Romce_and_romloe( gGet ) ; 
    } // else if
    else {
      Romce_and_romloe( gGet ) ;   
    } // else
    
  } // else
    
} // Rest_of_Identifier_started_basic_exp() 

void Rest_of_PPMM_Identifier_started_basic_exp( Token token ) {
 
  if ( gGet.tokenType == "[" ) {
    CutToken() ;
    Expression( gGet ) ;
    if ( gGet.tokenType != "]" )
      Error2( gGet.tokenName ) ;
    CutToken() ;  
  } // if
  
  Romce_and_romloe( gGet ) ;
  
} // Rest_of_PPMM_Identifier_started_basic_exp() 

void Actual_parameter_list( Token token ) {
    
  Basic_expression( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "," ) { 
    CutToken() ; 
    Basic_expression( gGet ) ; 
    CutToken() ;
  } // while
    
} // Actual_parameter_list()


void Romce_and_romloe( Token token ) {

      
  Rest_of_maybe_logical_OR_exp( gGet ) ;
  CutToken() ;
  if ( gGet.tokenType == "?" ) {
    CutToken() ;
    Basic_expression( gGet ) ;
    CutToken() ;
    if ( gGet.tokenType != ":" ) 
      Error2( gGet.tokenName ) ;
    CutToken() ;
    Basic_expression( gGet ) ; 
  } // if
  else {
    BackToken( gGet ) ;
  } // else
   
} // Romce_and_romloe()

void Rest_of_maybe_logical_OR_exp( Token token ) {

  Rest_of_maybe_logical_AND_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "||" ) {
    CutToken() ; 
    Maybe_logical_AND_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;
  
} // Rest_of_maybe_logical_OR_exp()

void Maybe_logical_AND_exp( Token token ) {

  Maybe_bit_OR_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "&&" ) {
    CutToken() ; 
    Maybe_bit_OR_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;
    
}  // Maybe_logical_AND_exp()

void Rest_of_maybe_logical_AND_exp( Token token ) {

  Rest_of_maybe_bit_OR_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "&&" ) {
    CutToken() ; 
    Maybe_bit_OR_exp( gGet ) ;
    CutToken() ;  
  } // while
    
  BackToken( gGet ) ;
    
} // Rest_of_maybe_logical_AND_exp()

void Maybe_bit_OR_exp( Token token ) {
   
  Maybe_bit_ex_OR_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "|" ) {
    CutToken() ; 
    Maybe_bit_ex_OR_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;
    
} // Maybe_bit_OR_exp()

void Rest_of_maybe_bit_OR_exp( Token token ) {

  Rest_of_maybe_bit_ex_OR_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "|" ) {
    CutToken() ; 
    Maybe_bit_ex_OR_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;   
    
} // Rest_of_maybe_bit_OR_exp()

void Maybe_bit_ex_OR_exp( Token token ) {

  Maybe_bit_AND_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "^" ) {
    CutToken() ; 
    Maybe_bit_AND_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Maybe_bit_ex_OR_exp()

void Rest_of_maybe_bit_ex_OR_exp( Token token ) {

  Rest_of_maybe_bit_AND_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "^" ) {
    CutToken() ; 
    Maybe_bit_AND_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;     
    
} // Rest_of_maybe_bit_ex_OR_exp()

void Maybe_bit_AND_exp( Token token ) {

  Maybe_equality_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "&" ) {
    CutToken() ; 
    Maybe_equality_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Maybe_bit_AND_exp()

void Rest_of_maybe_bit_AND_exp( Token token ) {
 
  Rest_of_maybe_equality_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "&" ) {
    CutToken() ; 
    Maybe_equality_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Rest_of_maybe_bit_AND_exp()

void Maybe_equality_exp( Token token ) {
   
  Maybe_relational_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "=" || gGet.tokenType == "!=" ) {
    CutToken() ; 
    Maybe_relational_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Maybe_equality_exp()

void Rest_of_maybe_equality_exp( Token token ) {
   
  Rest_of_maybe_relational_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "=" || gGet.tokenType == "!=" ) {
    CutToken() ; 
    Maybe_relational_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Rest_of_maybe_equality_exp()

void Maybe_relational_exp( Token token ) {
   
  Maybe_shift_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "<" || gGet.tokenType == ">" || gGet.tokenType == "<=" || 
          gGet.tokenType == ">=" ) {
    CutToken() ; 
    Maybe_shift_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;    
    
} // Maybe_relational_exp()

void Rest_of_maybe_relational_exp( Token token ) {
   
  Rest_of_maybe_shift_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "<" || gGet.tokenType == ">" || gGet.tokenType == "<=" || 
          gGet.tokenType == ">=" ) {
    CutToken() ; 
    Maybe_shift_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Rest_of_maybe_relational_exp()

void Maybe_shift_exp( Token token ) {
   
  Maybe_additive_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "<<" || gGet.tokenType == ">>" ) {
    CutToken() ; 
    Maybe_additive_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;   
    
} // Maybe_shift_exp()

void Rest_of_maybe_shift_exp( Token token ) {
    
  Rest_of_maybe_additive_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "<<" || gGet.tokenType == ">>" ) {
    CutToken() ; 
    Maybe_additive_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;     
    
} // Rest_of_maybe_shift_exp()

void Maybe_additive_exp( Token token ) {
    
  Maybe_mult_exp( gGet ) ;
  CutToken() ;
  while ( gGet.tokenType == "+" || gGet.tokenType == "-" ) {
    CutToken() ; 
    Maybe_mult_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;      
    
} // Maybe_additive_exp()

void Rest_of_maybe_additive_exp( Token token ) {
    
  Rest_of_maybe_mult_exp( gGet ) ; 
  CutToken() ;
  while ( gGet.tokenType == "+" || gGet.tokenType == "-" ) {
    CutToken() ; 
    Maybe_mult_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;     
    
} // Rest_of_maybe_additive_exp()

void Maybe_mult_exp( Token token ) {
 
  Unary_exp( gGet ) ;
  CutToken() ;
  Rest_of_maybe_mult_exp( gGet ) ; 
   
} // Maybe_mult_exp()
    
void Rest_of_maybe_mult_exp( Token token ) {
    
  while ( gGet.tokenType == "*" || gGet.tokenType == "/" || gGet.tokenType == "%" ) {
    CutToken() ; 
    Unary_exp( gGet ) ;
    CutToken() ;  
  } // while
  
  BackToken( gGet ) ;  
  
} // Rest_of_maybe_mult_exp()

void Unary_exp( Token token ) {
 
  if ( IsSign( token ) ) {
    CutToken() ;
    while ( IsSign( gGet ) ) {
      CutToken() ; 
    }  // while  
     
    Signed_unary_exp( gGet ) ; 
  } // if
  
  else if ( gGet.tokenType == "++" || gGet.tokenType == "--" ) {
    CutToken() ;
    if ( gGet.tokenType != "ident" )   
      Error2( gGet.tokenName ) ;
    CutToken() ;  
    if ( gGet.tokenType == "[" ) {
      CutToken() ;
      Expression( gGet ) ;
      if ( gGet.tokenType != "]" )   
        Error2( gGet.tokenName ) ;  
    } // if
    
    BackToken( gGet ) ;
      
  } // else if
  else 
    Unsigned_unary_exp( token ) ; 
  
} // Unary_exp()

void Signed_unary_exp( Token token ) {

  if ( gGet.tokenType == "ident" ) {
    if ( !HasDefined( gGet.tokenName ) ) {  
      Error3( gGet.tokenName ) ; 
    } // if  
    
    CutToken() ;
    if ( gGet.tokenType == "(" ) {
      CutToken() ; 
      if ( gGet.tokenType != ")" ) {
        CutToken() ; 
        Actual_parameter_list( gGet ) ;
        if ( gGet.tokenType != ")" ) 
          Error2( gGet.tokenName ) ;
      } // if 
        
    } // if
    else if ( gGet.tokenType == "[" ) {
      CutToken() ;  
      Expression( gGet ) ;
      if ( gGet.tokenType != "]" ) 
        Error2( gGet.tokenName ) ; 
    } // else if
    else {
      BackToken( gGet ) ;  
    } // else
  
  } // if
  
  else if ( gGet.tokenType == "const" ) {
    return ;  
  } // else if
   
  else if ( gGet.tokenType == "(" ) {
    CutToken() ;  
    Expression( gGet ) ;
    if ( gGet.tokenType != ")" ) 
      Error2( gGet.tokenName ) ;  
      
  } // else if
  else {
    Error2( gGet.tokenName ) ;     
  } // else

} // Signed_unary_exp()

void Unsigned_unary_exp( Token token ) {
  
  if ( gGet.tokenType == "ident" ) {
    if ( IsInstruction( gGet.tokenName ) ) {      // cin cout 特定function
    } // if
    else if ( gisFunction == 1 ) {                  // 定義function 
      if ( !HasDefined1( gGet.tokenName ) )  
        Error3( gGet.tokenName ) ; 
    } // else if
    else if ( gCompound == 1 ) { 
      if ( !HasDefined2( gGet.tokenName ) )
        Error3( gGet.tokenName ) ; 
    } // else if
    else if ( !HasDefined( gGet.tokenName ) ) {   // 一般定義  
      Error3( gGet.tokenName ) ; 
    } // if
    
    CutToken() ;
    if ( gGet.tokenType == "(" ) {  
      CutToken() ; 
      if ( gGet.tokenType != ")" ) {
        Actual_parameter_list( gGet ) ;
        if ( gGet.tokenType != ")" ) 
          Error2( gGet.tokenName ) ;
      } // if 
        
    } // if   
    else if ( gGet.tokenType == "[" ) {
      CutToken() ;  
      Expression( gGet ) ;
      if ( gGet.tokenType != "]" ) 
        Error2( gGet.tokenName ) ;
      CutToken() ;  
      if ( gGet.tokenType == "++" || gGet.tokenType == "--" ) {
        return ;    
      } // if
      else 
        BackToken( gGet ) ;    
      
       
    } // else if
    else if ( gGet.tokenType == "++" || gGet.tokenType == "--" )
      return ;
      
    else {
      BackToken( gGet ) ; 
    } // else
  
  } // if
  
  else if ( gGet.tokenType == "const" ) {
    return ;  
  } // else if
   
  else if ( gGet.tokenType == "(" ) {
    CutToken() ;  
    Expression( gGet ) ;
    if ( gGet.tokenType != ")" ) 
      Error2( gGet.tokenName ) ;  
      
  } // else if
  
  else {
    Error2( gGet.tokenName ) ;     
  } // else
 
} // Unsigned_unary_exp()


void CutToken() {
    
  char ch, next ;
  
  if ( gback.size() == 1 ) {
    gGet.column = gback[0].column ;
    gGet.row = gback[0].row ;
    gGet.tokenName = gback[0].tokenName ;
    gGet.tokenType = gback[0].tokenType ;
    gGet.value = gback[0].value ;
    gback.clear() ;
     
  } // if
  
  else {
    ch = getchar() ;
    next = cin.peek() ;
    
    gcolumn ++ ;

    while ( ch == ' ' || ch == '\t' || ch == '\n' || ( ch == '/' && next == '/' ) ) {
      
      if ( ch == ' ' || ch == '\t' || ch == '\n' ) {
        ReadTrash( ch ) ;  
        ch = getchar() ;  
        next = cin.peek() ;
      } // if
          
      else if ( ch == '/' && next == '/' ) {
        ReadAnnotation( ch, next ) ;
        ch = getchar() ;
        next = cin.peek() ;
      } // else if
      
    } // while     
      
    if ( IsDigital( ch ) || ch == '.' ) { 
      TokenDigital( ch ) ;  
    } // if
  
    else if ( IsLetter( ch ) || ch == '_' ) {   
      TokenLetter( ch ) ;
    } // else if
  
    else if ( IsSymbol( ch ) ) {    
      TokenSymbol( ch ) ;
    } // else if
  
    else if ( ch == '"' ) {  
      TokenDoubleQuotes( ch ) ;   
    } // else if
  
    else if ( ch == '\'' ) {
      TokenSingleQuotes( ch ) ;        
    } // else if
  
    else { 
      Error1( ch ) ;
    } // else
  
  } // else
    
} // CutToken() 

void ReadTrash( char ch ) { 

  char next = cin.peek() ;
  if ( ch == '\n' )
    grow ++ ;
      
  while ( next == ' ' || next == '\t' || next == '\n' ) {
    if ( next == '\n' ) {
      ch = getchar() ;
      grow ++ ;
      gcolumn = 0 ;
    } // if   
    else {  
      ch = getchar() ;
      gcolumn ++ ;  
    } // else
      
    next = cin.peek() ;
  } // while   

} // ReadTrash()
// 跳過空格換行TAB 

void ReadAnnotation( char ch, char next ) {  // 讀掉注解 
   
  if ( ch == '/' && next == '/' ) {
    ch = getchar() ;
    gcolumn ++ ;
      
    while ( ch != '\n' ) {
      ch = getchar() ;
      gcolumn ++ ;
        
    } // while
    
    gcolumn = 0 ;
    grow ++ ;

  } // if
 
} // ReadAnnotation()

void TokenDigital( char ch ) {   // 讀到數字進來判斷
 
  Token token ;
  char next = cin.peek() ;
  int row = grow ;
  int column = gcolumn ;
  string tType = "" ;  
  string tName = "" ; 
  float tValue = 0.0 ;
  bool dot = false ;
  
  tType = "const" ;
  if ( ch == '.' ) {   // .開頭 
    tName = tName + ch ;
    dot = true ;

    while ( IsDigital( next ) ) {
      ch = getchar() ; 
      gcolumn ++ ;
      tName = tName + ch ;
      next = cin.peek() ; 
    } // while
    
    tValue = atof( tName.c_str() ) ; 
  } // if
  
  else if ( IsDigital( ch ) ) {  
  
    if ( !IsDigital( next ) && next != '.' ) {        // 個位數 
      tName = tName + ch ;
      tValue = atof( tName.c_str() ) ;
    } // if
  
    else if ( IsDigital( next ) || next == '.' ) {   // 兩位數以上 
      tName = tName + ch ;

      while ( IsDigital( next ) || ( next == '.'  && dot == false ) ) {
        if ( next == '.' ) 
          dot = true ;
           
        ch = getchar() ; 
        gcolumn ++ ;
        tName = tName + ch ;
        next = cin.peek() ;
      } // while
    
      tValue = atof( tName.c_str() ) ; 
    } // else if
    
  } // else if

  SaveToken( tType, tName, tValue, row, column ) ;
  SaveTokenList( tType, tName, tValue, row, column ) ;
  
  gGet.tokenType = tType ;
  gGet.tokenName = tName ;
  gGet.value = tValue ;
  gGet.row = row ;
  gGet.column = column ;
  
} // TokenDigital()

void TokenLetter( char ch ) {  // 讀到'字母'or'_'近進判斷 

  Token token ;
  char next = cin.peek() ;
  int row = grow ;
  int column = gcolumn ;
  float tValue = 0.0 ;
  string tType = "" ;  
  string tName = "" ; 
  
  tName = tName + ch ;
  tType = "ident" ;
  tValue = 0.0 ;
  
  while ( IsLetter( next ) || IsDigital( next ) || next == '_' ) {
    ch = getchar() ;  
    tName = tName + ch ;
    gcolumn++ ;
    next = cin.peek() ;
  } // while
  
  TypeDetermine( tName, tType, tValue ) ;
  
  SaveToken( tType, tName, tValue, row, column ) ;
  SaveTokenList( tType, tName, tValue, row, column ) ;
  
  gGet.tokenType = tType ;
  gGet.tokenName = tName ;
  gGet.value = tValue ;
  gGet.row = row ;
  gGet.column = column ;

} // TokenLetter()

void TokenSymbol( char ch ) {  // 讀到Symbol進判斷 

  Token token ;
  char next = cin.peek() ;
  int row = grow ;
  int column = gcolumn ;
  float tValue = 0.0 ;
  string tType = "" ;  
  string tName = "" ; 
  
  if ( IsSymbol( next ) ) {
    if ( SymbolDetermine( ch, next ) ) {
      tName = tName + ch ;
      ch = getchar() ;
      gcolumn++ ;
      tName = tName + ch ;
      tType = tName ;    
    } // if
    else {
      tName = tName + ch ;
      tType = tName ;
    } // else
     
  } // if 
  
  else {
    tName = tName + ch ;
    tType = tName ;
  } // else
  
  SaveToken( tType, tName, tValue, row, column ) ;
  SaveTokenList( tType, tName, tValue, row, column ) ;

  gGet.tokenType = tType ;
  gGet.tokenName = tName ;
  gGet.value = tValue ;
  gGet.row = row ;
  gGet.column = column ;
  
} // TokenSymbol()

void TokenDoubleQuotes( char ch ) {  // 讀到'"'進判斷 

  Token token ;
  char next = cin.peek() ;
  int row = grow ;
  int column = gcolumn ;
  float tValue = 0.0 ;
  string tType = "" ;  
  string tName = "" ; 
  
  tType = "const" ;
  tName = tName + ch ;
  while ( next != '"'  ) {
    ch = getchar() ;
    gcolumn++ ;
    if ( ch == '\n' ) {
      gcolumn = 0 ;
      grow ++ ;
    } // if 
    
    tName = tName + ch ;
    next = cin.peek() ;
  } // while
  
  ch = getchar() ;
  tName = tName + ch ;

  SaveToken( tType, tName, tValue, row, column ) ;
  SaveTokenList( tType, tName, tValue, row, column ) ;
  
  gGet.tokenType = tType ;
  gGet.tokenName = tName ;
  gGet.value = tValue ;
  gGet.row = row ;
  gGet.column = column ;  

} // TokenDoubleQuotes()

void TokenSingleQuotes( char ch ) {  // 讀到'\''進判斷 

  Token token ;
  char next = cin.peek() ;
  int row = grow ;
  int column = gcolumn ;
  float tValue = 0.0 ;
  string tType = "" ;  
  string tName = "" ; 
  
  tType = "const" ;
  tName = tName + ch ;
  while ( next != '\''  ) {
    ch = getchar() ;
    gcolumn++ ;
    if ( ch == '\n' ) {
      gcolumn = 0;
      grow ++ ;
    } // if 
    
    tName = tName + ch ;
    next = cin.peek() ;
    
  } // while
  
  ch = getchar() ;
  tName = tName + ch ;
  
  SaveToken( tType, tName, tValue, row, column ) ;
  SaveTokenList( tType, tName, tValue, row, column ) ;
  
  gGet.tokenType = tType ;
  gGet.tokenName = tName ;
  gGet.value = tValue ;
  gGet.row = row ;
  gGet.column = column ;  

} // TokenSingleQuotes()

bool SymbolDetermine( char ch, char next ) {
  
  if ( ch == '*' || ch == '/' || ch == '%' || ch == '=' || ch == '!' ) {  // 只能接'=' 
    if ( next == '=' )
      return true ;
    else 
      return false ;
     
  } // if
  
  else if ( ch == '<' || ch == '>' || ch == '+' || ch == '-' ) {  // 可接自己或'=' 
    if ( next == ch || next == '=' )
      return true ;
    else
      return false ;  
  } // else if  
  
  else if ( ch == '&' || ch == '|' ) {  // '&&' '||'  
    if ( next == ch )
      return true ;
    else
      return false ; 
       
  } // else if
  else
    return false ;
    
} // SymbolDetermine()

void TypeDetermine( string tName, string &tType, float &tValue ) {

  if ( tName == "true" ) {
    tType = "const" ;
    tValue = 1.0 ;
  } // if 
  
  else if ( tName == "false" ) { 
    tType = "const" ;
    tValue = 0.0 ;
  } // else if
  
  else if ( tName == "int" ) 
    tType = "int" ;
  else if ( tName == "float" ) 
    tType = "float" ;
  else if ( tName == "char" ) 
    tType = "char" ;
  else if ( tName == "bool" ) 
    tType = "bool" ;
  else if ( tName == "string" ) 
    tType = "string" ;
  else if ( tName == "void" ) 
    tType = "void" ;
  else if ( tName == "if" ) 
    tType = "if" ;
  else if ( tName == "else" ) 
    tType = "else" ;
  else if ( tName == "while" ) 
    tType = "while" ;
  else if ( tName == "do" ) 
    tType = "do" ;
  else if ( tName == "return" ) 
    tType = "return" ;
  else 
    tType = "ident" ;

} // TypeDetermine()

bool IsDigital( char a ) {

  if ( ( int ) a >= 48 && ( int ) a <= 57  ) 
    return true ;
    
  else 
    return false ;

} // IsDigital()

bool IsLetter( char a ) {
    
  if ( ( ( int ) a >= 65 && ( int ) a <= 90 ) || ( ( int ) a >= 97 && ( int ) a <= 122 ) )
    return true ;
    
  else
    return false ;
    
} // IsLetter()

bool IsSymbol( char a ) {
    
  if ( a == '+' || a == '-' || a == '*' || a == '/' || a == '%' || a == '(' || 
       a == ')' || a == ';' || a == '=' || a == '>' || a == '<' || a == ':' || 
       a == '[' || a == ']' || a == '{' || a == '}' || a == '^' || a == '&' || 
       a == '|' || a == '?' || a == '!' || a == ',' ) 
    return true ;
    
  else
    return false ;
               
} // IsSymbol()

void Error1( char ch ) {
    
  string error ;
  error = error + ch ;
  gchoice = 1 ;
  throw error ;
    
} // Error1()

void Error2( string a ) {
    
  string error ;
  error = a ;
  gchoice = 2 ;
  throw error ;
    
} // Error2()

void Error3( string a ) {
    
  string error ;
  error = a ;
  gchoice = 3 ;
  throw error ;
    
} // Error3()

void SaveToken( string tType, string tName, float tValue, int row, int column ) {
    
  Token token ;  
  token.tokenType = tType ;
  token.tokenName = tName ;
  token.value = tValue ;
  token.row = row ;
  token.column = column ;
  gtoken.push_back( token ) ;
  
} // SaveToken()

void SaveTokenList( string tType, string tName, float tValue, int row, int column ) {
    
  Token token ;  
  token.tokenType = tType ;
  token.tokenName = tName ;
  token.value = tValue ;
  token.row = row ;
  token.column = column ;
  gtokenList.push_back( token ) ;
  
} // SaveTokenList()

void SaveIdent( Token gGet ) {
      
  Token token ;  
  token.tokenType = gGet.tokenType ;
  token.tokenName = gGet.tokenName ;
  token.value = gGet.value ;
  token.row = gGet.row ;
  token.column = gGet.column ;
  gIdent.push_back( token ) ;   
    
} // SaveIdent()

void SaveIdent1() {
  Token token ;    
  for ( int i = 0 ; i < gIdent.size() ; i ++ ) {
    token.tokenName = gIdent[i].tokenName ;
    token.tokenType = gIdent[i].tokenType ;
    token.value = gIdent[i].value ;
    token.column = gIdent[i].column ;
    token.row = gIdent[i].row ; 
    gIdent1.push_back( token ) ; 
  } // for   
    
} // SaveIdent1()

void SaveFuncToFunc2() {
  Token token ;    
  for ( int i = 0 ; i < gFunction.size() ; i ++ ) {
    token.tokenName = gFunction[i].tokenName ;
    token.tokenType = gFunction[i].tokenType ;
    token.value = gFunction[i].value ;
    token.column = gFunction[i].column ;
    token.row = gFunction[i].row ; 
    gFunction2.push_back( token ) ; 
  } // for   
    
} // SaveFuncToFunc2()



void StartMSG() {
    
  cout << "Our-C running ..." << endl ;
    
} // StartMSG()

void Test( ) {
    
  for ( int i = 0 ; i < gtokenList.size() ; i ++ ) {
    cout << gtokenList[i].tokenName << "  " << gtokenList[i].tokenType << "  " ; 
    cout << gtokenList[i].value << gtokenList[i].row << endl  ;
  } // for
  
} // Test()

int main() {

  int utestnum;
  cin >> utestnum ;
  Token token ;
  StartMSG() ;
  User_input() ;
  cin >> utestnum ;
  
} // main()
