#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;
string unit_input;
struct resister_data{//data type
	//string rst_name;
	vector<string>rs_set;
	vector<string>rp_set;//
	vector<double>rs_value;
	vector<double>rp_value;//
	vector<char>rs_ohm;
	vector<char>rp_ohm;
	vector<string>rs_p_nod;
	vector<string>rp_p_nod;
	vector<string>rs_n_nod;
	vector<string>rp_n_nod;
	double I_rs_up;
	double I_rp_down;
	string stage_nod;
}; 
vector<double>rs_idv_sum;
vector<double>rp_idv_sum;
//double I_rs_up[15];
//double I_rp_down[15];
double node_voltage[15];
double rst_cur[15];
//resister_data data;
//--------------------------------------------
vector<resister_data>my_vector(15);//最大的vector 
resister_data temp;
string find_unit_amp(double ans){
	if(unit_input=="K"){
		if(1000000<=ans&&ans<1000000000){
		return "KA";
		}
		else if(1000<=ans&&ans<1000000){
			return "A";
		}
		else if(1<=ans&&ans<1000){
			return "mA";
		}
		else if(0.001<=ans&&ans<1){
			return "uA";
		}
	}
else if(unit_input=="m"){
		if(1000000000<=ans&&ans<1000000000000){
		return "KA";
		}
		else if(1000000<=ans&&ans<1000000000){
			return "A";
		}
		else if(1000<=ans&&ans<1000000){
			return "mA";
		}
		else if(1<=ans&&ans<1000){
			return "uA";
		}
	}
	else if(unit_input=="Meg"){
		if(1000000<=ans&&ans<1000000000){
			return "Meg";
		} 
		else if(1000<=ans&&ans<1000000){
			return "KA";
		}
		else if(1<=ans&&ans<1000){
			return "A";
		}
		else if(0.001<=ans&&ans<1){
			return "mA";
		}
		else if(0.000001<=ans&&ans<0.001){
			return "uA";
		}
	}
	else if(unit_input=="G"){
		if(1000<=ans&&ans<1000000){
			return "Meg";
		} 
		else if(1<=ans&&ans<1000){
			return "KA";
		}
		else if(0.001<=ans&&ans<1){
			return "A";
		}
		else if(0.000001<=ans&&ans<0.001){
			return "mA";
		}
		else if(0.000000001<=ans&&ans<0.000001){
			return "uA";
		}
	}
}

//---------------------------------
string find_unit_vol(double ans){
	if(1000000<=ans&&ans<1000000000){
		return "Megv";
	}
	else if(1000<=ans&&ans<1000000){
		return "Kv";
	}
	else if(1<=ans&&ans<1000){
		return "v";
	}
	else if(0.001<=ans&&ans<1){
		return "mv";
	}
	else if(0.000001<=ans&&ans<0.001){
		return "uv";
	}
}
//-------------------------------------
double find_bit(double n){
	while(1){
		if(n>999){
			n/=1000;
		}
		if(n<1){
			n*=1000;
		}
		if(n>=1&&n<=999){
			break;
		}
	}
	return n;
}
void calculation (int stage_n,double v_all){
	//check
//	for(int i=0;i<rs_idv_sum.size();i++){
//		cout<<"rs_idv_sum[i]"<<rs_idv_sum[i]<<endl;
//	}
//	for(int i=0;i<rp_idv_sum.size();i++){
//		cout<<"rp_idv_sum[i]"<<rp_idv_sum[i]<<endl;
//	}
	//-------------------------------------------------------------
	double r_last_stage=rs_idv_sum[stage_n-1]+rp_idv_sum[stage_n-1];//
	rst_cur[stage_n-1]=r_last_stage;
	for(int i=stage_n-2;i>=0;i--){
		rst_cur[i]=(rp_idv_sum[i]*rst_cur[i+1])/(rp_idv_sum[i]+rst_cur[i+1])+rs_idv_sum[i];
	}
//	for(int i=0;i<stage_n;i++){
//		cout<<rst_cur[i]<<endl;
//	}
	double r_total=rst_cur[0];
	//cout<<"R total"<<r_total<<endl;
	my_vector[0].I_rs_up=v_all/r_total;
	node_voltage[0]=v_all-my_vector[0].I_rs_up*rs_idv_sum[0];
	for(int i=1;i<stage_n;i++){
		my_vector[i].I_rs_up=my_vector[i-1].I_rs_up*rp_idv_sum[i-1]/(rp_idv_sum[i-1]+rst_cur[i]);
		//cout<<"my_vector[i-1].I_rs_up"<<my_vector[i-1].I_rs_up<<endl;
	}
	for(int i=0;i<=stage_n;i++){
		my_vector[i].I_rp_down=my_vector[i-1].I_rs_up-my_vector[i].I_rs_up;
		//cout<<"my_vector[i].I_rp_down"<<my_vector[i].I_rp_down<<endl;
	}
	//count voltage
	for(int i=1;i<stage_n;i++){
		node_voltage[i]=node_voltage[i-1]-my_vector[i].I_rs_up*rs_idv_sum[i];
		//cout<<"node_voltage[i]"<<node_voltage[i]<<endl;
	}
	//----------------------------輸出-------------------------------------- 
	cout<<"Calculating... \nThe result is successfully calculated!"<< endl;
    cout<<"**************************************"<< endl;        
	cout<<"**           <<voltage>>            **"<< endl;
	cout<<setw(4)<<left<<"**";
	cout<<"vdd"<<"          "<<fixed<<setprecision(3)<<v_all<<"v"<<"             "<<"**"<<endl;
	for(int i=0;i<stage_n;i++){
		cout<<setw(4)<<left<<"**";
		if(i+1<10){
			cout<<my_vector[i].stage_nod<<"           "<<fixed<<setprecision(3)<<find_bit(node_voltage[i])<<find_unit_vol(node_voltage[i]);
		}
		else{
			cout<<my_vector[i].stage_nod<<"            "<<fixed<<setprecision(3)<<find_bit(node_voltage[i])<<find_unit_vol(node_voltage[i]);
		}
		cout<<"             **"<<endl;
	}
	cout<<"**           <<current>>            **"<<endl;
	for(int i=0;i<stage_n;i++){
		//cout<<setw(4)<<left<<"**";
		for(int j=0;j<my_vector[i].rs_set.size();j++){
			cout<<setw(4)<<left<<"**";
			cout<<my_vector[i].rs_set[j]<<"        "<<find_bit(my_vector[i].I_rs_up)<<find_unit_amp(my_vector[i].I_rs_up)<<"            **"<<endl;
		}
		for(int j=0;j<my_vector[i].rp_set.size();j++){
			cout<<setw(4)<<left<<"**";
			if(my_vector[i].rp_set[j].length()==3){
				cout<<my_vector[i].rp_set[j]<<"          "<<find_bit(my_vector[i+1].I_rp_down)<<find_unit_amp(my_vector[i+1].I_rp_down)<<"            **"<<endl;	
			}else{
				cout<<my_vector[i].rp_set[j]<<"        "<<find_bit(my_vector[i+1].I_rp_down)<<find_unit_amp(my_vector[i+1].I_rp_down)<<"            **"<<endl;
			}	
		}	
	}
	cout<<"**                                  **"<<endl;
	cout<<"**                   myspice2021    **"<<endl;
	cout<<"**************************************"<< endl;
	//cout<<endl;
}

void swap_stage(int stage_n){
	int stage_1_2,stage_2_2;
	int stage_1,stage_2;
	//-----------------------防呆------------------------ 
	while(1){
		cout<<"Please enter the first stage you want to swap: ";
		cin>>stage_1_2;
		if(stage_1_2<=stage_n){
			break;
		}
		else{
			cout<<"The node doesn’t exist. Please enter again."<<endl;
			continue;
		}
	}
	while(1){
		cout<<"Please enter the second stage you want to swap: ";
		cin>>stage_2_2;
		if(stage_2_2<=stage_n){
			break;
		}
		else{
			cout<<"The node doesn’t exist. Please enter again."<<endl;
			continue;
		}
	}
	if(stage_2_2>stage_1_2){
		stage_2=stage_2_2;
		stage_1=stage_1_2;
	}
	else{
		stage_1=stage_2_2;
		stage_2=stage_1_2;
	}
	resister_data temp;
	//cout<<"要是vdd"<<my_vector[0].rp_p_nod[0]<<endl;
//	cout<<"原本"<<endl;
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rp_value.size();j++){
//		cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;				
//		}	
//	}
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rs_value.size();j++){
//		cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_value[j]<<endl;				
//		}	
//	}
	//----------------------------------------------- 
	cout<<my_vector[stage_1-1].rs_set[0]<<" is changing node from "<<my_vector[stage_1-1].rs_p_nod[0];
	temp=my_vector[stage_1-1];
	my_vector[stage_1-1]=my_vector[stage_2-1];
	cout<<" to "<<my_vector[stage_1-1].stage_nod<<endl;
	cout<<my_vector[stage_2-1].rs_set[0]<<" is changing node from "<<my_vector[stage_2-1].rs_p_nod[0];
	//-----------
	my_vector[stage_2-1]=temp;
	//cout<<"temp.stage_nod"<<temp.stage_nod<<endl;
	cout<<" to "<<my_vector[stage_2-1].rs_p_nod[0]<<endl;
	//-----------------
	my_vector[stage_1-1].rs_p_nod[0]=my_vector[stage_2-1].rs_p_nod[0];
	my_vector[stage_2-1].rs_p_nod[0]=my_vector[stage_1-1].stage_nod;
	
	if(stage_2<stage_n){
		cout<<my_vector[stage_2].rs_set[0]<<" is changing node from "<<my_vector[stage_2].rs_p_nod[0];
		my_vector[stage_2].rs_p_nod[0]=my_vector[stage_2-1].stage_nod;
		//cout<<"my_vector[stage_2-1].stage_nod"<<my_vector[stage_2-1].stage_nod<<endl;
		cout<<" to "<<my_vector[stage_2].rs_p_nod[0]<<"..."<<endl;
	}
	double temp_rs_sum;
	temp_rs_sum=rs_idv_sum[stage_1-1];
	rs_idv_sum[stage_1-1]=rs_idv_sum[stage_2-1];
	rs_idv_sum[stage_2-1]=temp_rs_sum;
	//------------------------
	double temp_rp_sum;
	temp_rp_sum=rp_idv_sum[stage_1-1];
	rp_idv_sum[stage_1-1]=rp_idv_sum[stage_2-1];
	rp_idv_sum[stage_2-1]=temp_rp_sum;
	
	cout<<"The stage is Successfully swapped!"<<endl;
	//---------------------------------------------------------------
	//cout<<"後來"<<endl;
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rp_value.size();j++){
//		cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;				
//		}	
//	}
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rs_value.size();j++){
//		cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_value[j]<<endl;				
//		}	
//	}
//	for(int i=0;i<stage_n;i++){
//		cout<<"rs_idv_sum[i]"<<rs_idv_sum[i]<<endl;
//	}
//	for(int i=0;i<stage_n;i++){
//		cout<<"rp_idv_sum[i]"<<rp_idv_sum[i]<<endl;
//	}
}

void merge_stage (int &stage_n){
	int mrg_stg_a,mrg_stg_b;
	
	while(1){
		cout<<"Please enter the first stage you want to merge: ";
		cin>>mrg_stg_a;
		if(mrg_stg_a<=stage_n){
			break;
		}
		else{
			cout<<"The node doesn’t exist. Please enter again."<<endl;
			continue;
		}
	}
	while(1){
		cout<<"Please enter the second stage you want to merge: ";
		cin>>mrg_stg_b;
		if(mrg_stg_b<=stage_n){
			break;
		}
		else{
			cout<<"The node doesn’t exist. Please enter again."<<endl;
			continue;
		}
	}
	//-------------------------------------------------------------
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rs_value.size();j++){
//		cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_value[j]<<endl;				
//		}			
//	}
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rp_value.size();j++){
//		cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;				
//		}			
//	}
	//--------------------------------------------------------------
	//cout<<"計算過程 rp_idv_sum[mrg_stg_a-1], rp_idv_sum[mrg_stg_b-1], rs_idv_sum[mrg_stg_b-1]"
			//<<rp_idv_sum[mrg_stg_a-1]<<" "<<rp_idv_sum[mrg_stg_b-1]<<" "<<rs_idv_sum[mrg_stg_b-1]<<endl;
	//cout<<"rp_idv_sum[mrg_stg_a-1]前 "<<rp_idv_sum[mrg_stg_a-1]<<endl; 
	rp_idv_sum[mrg_stg_a-1]=rp_idv_sum[mrg_stg_a-1]*(rp_idv_sum[mrg_stg_b-1]+rs_idv_sum[mrg_stg_b-1])/(rp_idv_sum[mrg_stg_a-1]+rp_idv_sum[mrg_stg_b-1]+rs_idv_sum[mrg_stg_b-1]);
	//cout<<"rp_idv_sum[mrg_stg_a-1]後 "<<rp_idv_sum[mrg_stg_a-1]<<endl; 
	my_vector[mrg_stg_a-1].rp_value[0]=rp_idv_sum[mrg_stg_a-1];
	string index= to_string(mrg_stg_a);
	
	my_vector[mrg_stg_a-1].rp_set[0]="RP"+index;
	string rp_name=my_vector[mrg_stg_a-1].rp_set[0];
	double rp_value2=rp_idv_sum[mrg_stg_a-1];//暫存一下 
//	cout<<"算完後"<<endl; 
//	cout<<"my_vector[mrg_stg_a-1].rp_value[0] "<<my_vector[mrg_stg_a-1].rp_value[0]<<endl;
//	cout<<"my_vector[mrg_stg_a-1].rp_set[0]"<<my_vector[mrg_stg_a-1].rp_set[0]<<endl;
	cout<<"Resistors are merged into "<<my_vector[mrg_stg_a-1].rp_set[0]<<"."<<endl;
	cout<<"=> "<<my_vector[mrg_stg_a-1].rp_set[0]<<" "<<my_vector[mrg_stg_a-1].rp_p_nod[0]<<" gnd "<<fixed<<setprecision(3)<<my_vector[mrg_stg_a-1].rp_value[0]<<" "<<unit_input<<endl;
	cout<<"The stage is Successfully merged!"<<endl;
	//my_vector.pop_back();
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rp_set.size();j++){
//			cout<<"目前stage"<<i+1<<" ";
//				cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;;
//			}
//		}
	//cout<<"here"<<endl;
	my_vector.erase(my_vector.begin()+mrg_stg_b-1);
	//cout<<"my_vector.size()"<<my_vector.size()<<endl;
	my_vector[mrg_stg_a-1].rp_set.clear();
	my_vector[mrg_stg_a-1].rp_value.clear();
	my_vector[mrg_stg_a-1].rp_set.push_back(rp_name);
	my_vector[mrg_stg_a-1].rp_value.push_back(rp_value2);
	rs_idv_sum.erase(rs_idv_sum.begin()+mrg_stg_b-1);
	rp_idv_sum.erase(rp_idv_sum.begin()+mrg_stg_b-1);
	//------------------------------------------------------------------
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rp_value.size();j++){
//		cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;				
//		}	
//	}
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rs_value.size();j++){
//		cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_value[j]<<endl;				
//		}	
//	}
	stage_n--;
	my_vector[mrg_stg_a-1].rp_n_nod[0]="gnd";
//	for(int i=0;i<my_vector.size();i++){
//		for(int j=0;j<my_vector[i].rs_value.size();j++){
//		cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_value[j]<<endl;				
//		}	
//	}
	
}
int main(){
	int loop_bol=1;
	int read_or_not=0;
	cout<<"*** Welcome to MySpice ***"<<endl;
	//resister_data data;//以後data就是物件實體了 
	resister_data temp;
	string filename;
	double vol_value;
	int stage_num;//有幾個stage 
	string notuse1,notuse2,notuse3,notuse4,notuse5;
	int slc;
	int stage_1,stage_2;
	//---------------------------------------------
	//vector<resister_data>my_vector;//最大的vector 
	//選單 
	while(loop_bol==1){
		cout<<"=========================================="<<endl;
		cout<<"Please type 1 to input file"<<endl;
		cout<<"Please type 2 to calculate voltage and current"<<endl;
		cout<<"Please type 3 to swap stage"<<endl;
		cout<<"Please type 4 to merge stage"<<endl;
		cout<<"Please type 5 to output file"<<endl;
		cout<<"Please type 6 to exit"<<endl;
		cout<<"Enter your selection Here: ";
		cin>>slc;
		cout<<"=========================================="<<endl;
		//cout<<"Calculating... "<<endl;
		//cout<<"The result is successfully calculated!"<<endl;
		//cout列表
		switch(slc){
			case 1:{
				cout<<"Please enter the name of the input file: ";
a:				cin>>filename;
				ifstream input_file(filename,ios::in);
				if(!input_file){
					cout<<"The file is not found. "<<endl;
					cout<<"Please enter the name of the input file again:";
					goto a;
					}
				cout<<"Loading the file... "<<endl;
				cout<<"The input file successfully loaded!"<<endl;
				//-----------------------------以下開始讀檔------------------------------------------------------
				input_file>>notuse1>>stage_num>>notuse2>>notuse3>>notuse4>>vol_value>>notuse5;
				string rst_name;
				string p_nod,n_nod;
				double rst_value;
				char ohm;
				while(1){//my_vector[stage_1-1].rst_name
					input_file>>rst_name>>p_nod>>n_nod>>rst_value>>unit_input;
					if(rst_name[1]=='S'){
 						int i=static_cast<int>(rst_name[2])-49; 
						my_vector[i].rs_set.push_back(rst_name);
						my_vector[i].rs_p_nod.push_back(p_nod);
						my_vector[i].rs_n_nod.push_back(n_nod);
						my_vector[i].rs_value.push_back(rst_value);
						//my_vector[i].rs_ohm.push_back(ohm);
//						my_vector.rs_set[i].push_back(data);
					}
					else if(rst_name[1]=='P'){
						int i=static_cast<int>(rst_name[2]-49);
						my_vector[i].rp_set.push_back(rst_name);
						my_vector[i].rp_p_nod.push_back(p_nod);
						my_vector[i].rp_n_nod.push_back(n_nod);
						my_vector[i].rp_value.push_back(rst_value);
						//my_vector[i].rp_ohm.push_back(ohm);
						//my_vector.rp_set[i].push_back(data);
					}
					if(rst_name=="end"){
						break;
					}
				}
			//-----------------------初始化----------------------- 
			for(int i=0;i<stage_num;i++){ 
				double r_sum=0;
				for(int j=0;j<my_vector[i].rs_value.size();j++){
					r_sum+=my_vector[i].rs_value[j];
					}
					rs_idv_sum.push_back(r_sum);
			}
				
			for(int i=0;i<stage_num;i++){
				double r_sum=0;
				for(int j=0;j<my_vector[i].rp_value.size();j++){
					r_sum+=my_vector[i].rp_value[j];
				}
					rp_idv_sum.push_back(r_sum);
			}
			string index;
			//my_vector[0].stage_nod="Vdd";
			for(int i=0;i<stage_num;i++){
				index= to_string(i+1);
				my_vector[i].stage_nod="n"+index;
				//cout<<"my_vector[i].stage_nod"<<my_vector[i].stage_nod<<endl;
			}
			
//				for(int i=0;i<my_vector.size();i++){
//					for(int j=0;j<my_vector[i].rs_p_nod.size();j++){
//						cout<<my_vector[i].rs_set[j]<<" "<<my_vector[i].rs_p_nod[j]<<endl;
//					}
//			}
			//cout<<"要是vdd"<<my_vector[0].rp_p_nod[0]<<endl;	
//				double r_total=0;
//				for(int i=0;i<my_vector[stage_num-1].rp_value.size();i++){
//						cout<<"my_vector[stage_n].rp_value[i]"<<my_vector[stage_num-1].rp_value[i]<<endl;
//						r_total+=my_vector[stage_num-1].rp_value[i];
//					}
//				cout<<r_total;

//				for(int i=0;i<my_vector.size();i++){
//					for(int j=0;j<my_vector[i].rp_set.size();j++){
//						cout<<"目前stage"<<i+1<<" ";
//						cout<<my_vector[i].rp_set[j]<<" "<<my_vector[i].rp_value[j]<<endl;;
//					}
//				}
				read_or_not=1;
				break;
			}
			case 3:
				if(read_or_not==0){
					cout<<"There is no circuit."<<endl;
				}
				else{
					swap_stage(stage_num);
				}
				break;
			case 2:
				if(read_or_not==0){
					cout<<"There is no circuit."<<endl;
				}
				else{
					calculation (stage_num,vol_value);
				}
				break;
			case 4:
				if(read_or_not==0){
					cout<<"There is no circuit."<<endl;
				}
				else{
					merge_stage (stage_num);
				}
				break;
			case 6:
				cout<<"Bye~"<<endl;
				loop_bol=0;
				break;
			case 5:
				if(read_or_not==0){
					cout<<"There is no circuit."<<endl;
				}
				else{

				string outfilename;
				cout<<"Please enter the name of the output file: ";
				cin>>outfilename;
				cout<<"Exporting the file... "<<endl;
				cout<<"The output file successfully exported!"<<endl;
				ofstream outfile(outfilename,ios::out);
				//calculation (stage_num,vol_value);
				outfile<<"***circuits***"<<endl;
				outfile<<"stage "<<stage_num<<endl<<endl;
				outfile<<"v1 vdd gnd "<<vol_value<<"V"<<endl<<endl;
				for(int i=0;i<my_vector.size();i++){
					for(int j=0;j<my_vector[i].rs_set.size();j++){
						outfile<<left<<setw(8)<<my_vector[i].rs_set[j]<<left<<setw(8)
							<<my_vector[i].rs_p_nod[j]<<left<<setw(8)<<my_vector[i].rs_n_nod[j]<<fixed<<setprecision(3)<<my_vector[i].rs_value[j]<<unit_input<<endl;
					}
					for(int j=0;j<my_vector[i].rp_set.size();j++){
						outfile<<left<<setw(8)<<my_vector[i].rp_set[j]<<left<<setw(8)
							<<my_vector[i].rp_p_nod[j]<<left<<setw(8)<<my_vector[i].rp_n_nod[j]<<fixed<<setprecision(3)<<my_vector[i].rp_value[j]<<unit_input<<endl;
					}
				}
				outfile<<endl;
				outfile<<"***calculation results***"<< endl;
			    outfile<<"**************************************"<< endl;        
				outfile<<"**           <<voltage>>            **"<< endl;
				outfile<<setw(4)<<left<<"**";
				outfile<<"vdd"<<"          "<<fixed<<setprecision(3)<<vol_value<<"v"<<"             "<<"**"<<endl;
				for(int i=0;i<stage_num;i++){
					outfile<<setw(4)<<left<<"**";
					if(i+1<10){
						outfile<<my_vector[i].stage_nod<<"           "<<fixed<<setprecision(3)<<find_bit(node_voltage[i])<<find_unit_vol(node_voltage[i]);
					}
					else{
						outfile<<my_vector[i].stage_nod<<"            "<<fixed<<setprecision(3)<<find_bit(node_voltage[i])<<find_unit_vol(node_voltage[i]);
					}
					outfile<<"             **"<<endl;
				}
				outfile<<"**           <<current>>            **"<<endl;
				for(int i=0;i<stage_num;i++){
					//cout<<setw(4)<<left<<"**";
					for(int j=0;j<my_vector[i].rs_set.size();j++){
						outfile<<setw(4)<<left<<"**";
						outfile<<my_vector[i].rs_set[j]<<"        "<<find_bit(my_vector[i].I_rs_up)<<find_unit_amp(my_vector[i].I_rs_up)<<"            **"<<endl;
					}
					for(int j=0;j<my_vector[i].rp_set.size();j++){
						outfile<<setw(4)<<left<<"**";
						if(my_vector[i].rp_set[j].length()==3){
							outfile<<my_vector[i].rp_set[j]<<"          "<<find_bit(my_vector[i+1].I_rp_down)<<find_unit_amp(my_vector[i+1].I_rp_down)<<"            **"<<endl;	
						}else{
							outfile<<my_vector[i].rp_set[j]<<"        "<<find_bit(my_vector[i+1].I_rp_down)<<find_unit_amp(my_vector[i+1].I_rp_down)<<"            **"<<endl;
						}	
					}	
				}
				outfile<<"**                                  **"<<endl;
				outfile<<"**                   myspice2021    **"<<endl;
				outfile<<"**************************************"<< endl;
				outfile<<endl;
				break;
			}
		}	 
	}
	return 0;
}
