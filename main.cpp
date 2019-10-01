#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Mesh.h"

using namespace std;

void DisplayHead()
{
   const string infor =
      "\nGmsh2TIN, convert Gmsh triangular mesh to TIN file.\n "
      "By Wenqing Wang\n\n"
      "Command: gmsh2TIN [option] [File name without extension].\n"
      "option: -ogmsh"
      "        Output a new Gmsh file for validation.\n";
   cout << infor << endl;
}

bool parseCLI_Option(const std::vector<std::string> &cmd_args,
                     std::string &file_name)
{
   const size_t nargs = cmd_args.size();

   bool has_option = false;

   for(size_t i=0; i<nargs; i++)
   {
      std::string s_buff = cmd_args[i];
      if(s_buff.find("--help")!=string::npos )
      {
         DisplayHead();
         exit(EXIT_SUCCESS);
      }

      if( s_buff.find("-ogmsh") != string::npos)
      {
         has_option = true;
      }

      if(   !(s_buff.find("-")!=string::npos || s_buff.find("--")!=string::npos) )
      {
         file_name = s_buff;
      }
   }

   return has_option;
}

//
int main(int argc, char* argv[])
{
   std::vector<std::string> cmd_args;

   if(argc>1)
   {
      cmd_args.insert(cmd_args.begin(), argv+1, argv+argc);
   }
   else //terminal
   {
      DisplayHead();
      cout << "\nInput options and file name (non extension):\n ";

      std::string s_buff;
      getline(cin, s_buff);
      stringstream ss;
      ss.str(s_buff);
      while(!ss.eof())
      {
         ss>>s_buff;
         cmd_args.push_back(s_buff);
      }
      ss.clear();
   }

   string file_name;
   const bool o_gmsh  = parseCLI_Option(cmd_args, file_name);

   // Name and path of the input data file
   const auto index_Win = file_name.find_last_of('\\');
   const auto index_Linux = file_name.find_last_of('/');

   //
   string file_path = "";
   if(index_Win != string::npos)
   {
      file_path = file_name.substr(0, index_Win) + "\\";
   }
   else if(index_Linux != string::npos)
   {
      file_path = file_name.substr(0, index_Linux) + "/";
   }

   Mesh mesh;
   if( !mesh.read(file_name + ".msh") )
   {
      return EXIT_FAILURE;
   }

   if( !mesh.write(file_name + ".tin") )
   {
      return EXIT_FAILURE;
   }

   if(o_gmsh)
   {
      if( !mesh.writeGmsh(file_name + "_for_validation.msh") )
      {
         return EXIT_FAILURE;
      }
   }

   return EXIT_SUCCESS;
}


