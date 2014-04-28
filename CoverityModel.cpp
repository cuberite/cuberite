

extern "C" { 
  struct lua_State;
  struct tolua_Error
  {
          int index;
          int array;
          const char* type;
  };
  
  void tolua_error (lua_State* L, const char* msg, tolua_Error* err)
  {
    __coverity_panic__();
  }
  
}
