#include "config_file_reader.h"



config_file_reader::config_file_reader(string filename){
	outputfilename = "output.root";

	ifstream config (filename);

	string line;
	string par = "parameter";
	string comment = "//";
	string double_forward_slash = comment;
	string semi = ";";
	string read_root = "readROOTfile";
	string write_root = "set_root_output_file_name";
	string back_slash; // = "\\"; 
	back_slash.push_back(92);
	string double_quotes;
	double_quotes.push_back(34);
	string dot_root = ".root";
	string space = " ";
	string open_comment =  "/*";
	string close_comment = "*/";

	size_t found_par;
	size_t found_comment;
	size_t found_semi;
	size_t found_rroot;
	size_t found_wroot;
	size_t found_bslash;
	size_t found_dqoutes;
	size_t found_dotroot;
	size_t found_pound;
	size_t found_space;
	size_t found_Open_comment;
	size_t found_Close_comment;

	bool multi_line_comment = false;


	
	
	int par_num = -1000;
	double par_value = -1000;



	if (config.is_open())
	{
		while(!config.eof())
		{
			getline(config, line);

			if(multi_line_comment){
				found_Close_comment = line.find(close_comment);
				if (found_Close_comment!=string::npos){
					multi_line_comment=false;
					line = line.substr(found_Close_comment+2);
					//cout << "found close comment. multi_line_comment= " << multi_line_comment<< endl;
				}
			}
			if(multi_line_comment == false){
				found_Open_comment = line.find(open_comment);
				if (found_Open_comment!=string::npos){
					line = line.substr(0, found_Open_comment);
					multi_line_comment = true;
					//cout << "found open comment. multi_line_comment= " << multi_line_comment<< endl;
				}

				// remove the comments
				found_comment = line.find(comment);
				if (found_comment!=string::npos)	
					line = line.substr(0,found_comment);
				// remove the semi colons 
				found_semi = line.find(semi);
				if (found_semi!=string::npos)	
					line = line.erase(found_semi,1);
		
				//remove the parameter and then fill parameter map
				found_par = line.find(par);
				if (found_par!=string::npos){
					line = line.erase(found_par,9);
		
					stringstream line_stream(line);
					line_stream >> par_num >> par_value;
					parameter[par_num]=par_value;
					//printf("parameter %i	%G \n" , par_num, parameter[par_num]);

				}

		/////////////////////////////////// insert presorter commands here /////////////////////////////////////////////////
				string new_presorter="new_presorter";
				size_t found_new_presorter;
				found_new_presorter = line.find(new_presorter); //try to find "new_presorter"
				if (found_new_presorter!=string::npos){

					line = line.erase(found_new_presorter, new_presorter.size()); // erase the "new_presorter"
					command temp_command;		// create temp commad object 
					string presorter;									
					size_t found_presorter;
					int num_arg = 0;
					double temp_arg = 0;
					
					
					//ELECTRONTOF
					presorter ="ELECTRONTOF";
					num_arg = 3;
					found_presorter = line.find(presorter);				//try to find "ELECTRONTOF"
					if (found_presorter!=string::npos){					//if found do something
						temp_command.command_str=presorter;				// store which command string was found
						line = line.erase(found_presorter, presorter.size()); // erase the "ELECTRONTOF"
						
						stringstream line_stream(line);

						for(int i=0; i < num_arg; ++i){
							line_stream >> temp_arg;				// get the number from the string
							temp_command.arg.push_back(temp_arg);				// store the argument for ELECTRONTOF in the arguments vector
						}
						
						config_commands.push_back(temp_command);		// store the eTOF command object in the a vector. 
						cout << "Persorter found: " <<config_commands[0].command_str <<" "<< config_commands[0].arg[0] <<" "<< config_commands[0].arg[1] <<" "<< config_commands[0].arg[2] << endl;
					}

				}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				else{
					//look for readROOTfile
					found_rroot = line.find(read_root);
					if (found_rroot!=string::npos){
						line = line.erase(found_rroot, read_root.size());

						////replace \ with \\ for file path
						//found_bslash = line.find(back_slash);
						//while (found_bslash!=string::npos){
						//	line.replace(found_bslash, 1, "#");  // marking the spot with a pound sign (i know it's a crappy way to do it)
						//	found_bslash = line.find(back_slash);
						//}
						//found_pound = line.find("#");
						//while (found_pound!=string::npos){
						//	line.replace(found_pound, 1,back_slash + back_slash);  
						//	found_pound = line.find("#");
						//}

						// find and remove anything before and after the double quotes
						found_dqoutes = line.find(double_quotes);
						if (found_dqoutes!=string::npos){
							line.erase(found_dqoutes, 1);
							line = line.substr(found_dqoutes, line.find(double_quotes)-found_dqoutes);
						}
						//cout << line << endl;
						inputfilename.push_back(line);
					}

					//look for set_root_output_file_name
					found_wroot = line.find(write_root);
					if (found_wroot!=string::npos){
						line = line.erase(found_wroot,write_root.size());

						////replace \ with \\ for file path
						//found_bslash = line.find(back_slash);
						//while (found_bslash!=string::npos){
						//	line.replace(found_bslash, 1, "#");  // marking the spot with a pound sign
						//	found_bslash = line.find(back_slash);
						//}
						//found_pound = line.find("#");
						//while (found_pound!=string::npos){
						//	line.replace(found_pound, 1, "\\");  
						//	found_pound = line.find("#");
						//}
						// find and remove "
						found_dqoutes = line.find(double_quotes);
						if (found_dqoutes!=string::npos){
							line.erase(found_dqoutes, 1);
							line = line.substr(found_dqoutes, line.find(double_quotes)-found_dqoutes);
						}
						outputfilename = line;

						// add .root to the end of the file name if it isn't there
						found_dotroot = outputfilename.find(dot_root);
						if (found_dotroot==string::npos){
							outputfilename.append(dot_root);
						}
					
						//cout << write_root << outputfilename << endl;
					}

				}
			}
		}//end of while(!config.eof())
	}

	config.close();
}

	
config_file_reader::~config_file_reader(){}


