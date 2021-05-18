#include <iostream>
#include <string>
#include "AESObject.h"
#include "Precompute.h"
#include "secondary.h"
#include "connect.h"
#include "NeuralNetConfig.h"
#include "NeuralNetwork.h"
#include "unitTests.h"
#include <fstream>



int partyNum;
AESObject* aes_indep;
AESObject* aes_next;
AESObject* aes_prev;
Precompute PrecomputeObject;
// extern float LAN_ping;
// extern float LAN_Com;
// extern float WAN_ping;
// extern float WAN_Com;
extern int MatMul_Com,ReLU_Com,MP_Com,MP_ReLU_Com,Div_Com,BN_Com,PP_Com,SS_Com;
extern int MatMul_rounds,ReLU_rounds,MP_rounds,Div_rounds,BN_rounds,PP_rounds,SS_rounds;
extern int MatMul_time,ReLU_time,MP_time,MP_ReLU_time,Div_time,BN_time;
extern CommunicationObject commObject;
int main(int argc, char** argv)
{
	string Network_l[6]={"SecureML", "Sarda", "MiniONN", "LeNet", "AlexNet", "VGG16"};
	string Dataset_l[3]={"MNIST", "CIFAR10", };//"ImageNet"};
	string network, dataset, security;
	security = "Semi-honest";
	for (int networki = 0; networki < 6; networki++) {
		for (int dataseti = 0; dataseti < 2; dataseti++){

			network = Network_l[networki];
			dataset = Dataset_l[dataseti];
			// cout << network << dataset << (network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")
			// <<(dataset!="MNIST")<<((network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")&&(dataset!="MNIST"))
			//  << endl;
			if((network=="SecureML"||network=="Sarda"||network=="MiniONN"||network=="LeNet")&&(dataset!="MNIST")){
				continue;
			} 
			else if ((network=="AlexNet"||network=="VGG16")&&(dataset=="MNIST")){
				continue;
			}
			// else if (dataset=="MNIST"||(dataset=="CIFAR10")){
			// 	continue; //TODO: to be deleted
			// }
			cout << network << dataset << security<< endl;

		/****************************** PREPROCESSING ******************************/ 
			parseInputs(argc, argv);
			NeuralNetConfig* config = new NeuralNetConfig(NUM_ITERATIONS);
			
			bool PRELOADING = false;

		/****************************** SELECT NETWORK ******************************/ 
			//Network {SecureML, Sarda, MiniONN, LeNet, AlexNet, and VGG16}
			//Dataset {MNIST, CIFAR10, and ImageNet}
			//Security {Semi-honest or Malicious}
			// if (argc == 9)
			// {network = argv[6]; dataset = argv[7]; security = argv[8];}
			// else
			// {
			// 	network = "AlexNet";//"VGG16";
			// 	dataset = "CIFAR10";//"CIFAR10";
			// 	security = "Semi-honest";
			// }
			selectNetwork(network, dataset, security, config);
			config->checkNetwork();

			NeuralNetwork* net = new NeuralNetwork(config);

		/****************************** AES SETUP and SYNC ******************************/ 
			aes_indep = new AESObject(argv[3]);
			aes_next = new AESObject(argv[4]);
			aes_prev = new AESObject(argv[5]);
			cout<<1<<endl;
			initializeCommunication(argv[2], partyNum);
			cout<<1<<endl;
			synchronize(2000000);
			cout<<1<<endl;

		/****************************** RUN NETWORK/UNIT TESTS ******************************/ 
			//Run these if you want a preloaded network to be tested
			//assert(NUM_ITERATION == 1 and "check if readMiniBatch is false in test(net)")
			//First argument {SecureML, Sarda, MiniONN, or LeNet}
			// network += " preloaded"; PRELOADING = true;
			// preload_network(PRELOADING, network, net);

			start_m();
			//Run unit tests in two modes: 
			//	1. Debug {Mat-Mul, DotProd, PC, Wrap, ReLUPrime, ReLU, Division, BN, SSBits, SS, and Maxpool}
			//	2. Test {Mat-Mul1, Mat-Mul2, Mat-Mul3 (and similarly) Conv*, ReLU*, ReLUPrime*, and Maxpool*} where * = {1,2,3}
			// runTest("Debug", "BN", network);
			// runTest("Test", "ReLUPrime1", network);

			// Run forward/backward for single layers
			//  1. what {F, D, U}
			// 	2. l {0,1,....NUM_LAYERS-1}
			// size_t l = 0;
			// string what = "F";
			// runOnly(net, l, what, network);

			// //Run training
			// network += " train";
			// train(net);

			//Run inference (possibly with preloading a network)
			network += " test";
			test(PRELOADING, network, net);
			ofstream myfile;
			string filename;
			filename = "Inf_Falcon_comm_"+security+"_"+network+"_"+dataset+"_"+".txt";
			myfile.open (filename.c_str());
			myfile << "----------------------------------------------" << endl;  	
			myfile << "Run details: " << NUM_OF_PARTIES << "PC (P" << partyNum 
				<< "), " << NUM_ITERATIONS << " iterations, batch size " << MINI_BATCH_SIZE << endl 
				<< "Running " << security << " " << network << " on " << dataset << " dataset" << endl;
			myfile << "----------------------------------------------" << endl << endl; 
			myfile.close();	
			string filename1 = "Inf_Falcon_comm_"+security+"_"+network+"_"+dataset+"_"+"1.txt";
			myfile.open (filename1.c_str());
			myfile << endl;
			myfile.close();	
			end_m(network,filename1);
			// cout << "----------------------------------------------" << endl;  	
			// cout << "Run details: " << NUM_OF_PARTIES << "PC (P" << partyNum 
			// 	 << "), " << NUM_ITERATIONS << " iterations, batch size " << MINI_BATCH_SIZE << endl 
			// 	 << "Running " << security << " " << network << " on " << dataset << " dataset" << endl;
			// cout << "----------------------------------------------" << endl << endl;  

			// cout << "MatMul_Com: "<< MatMul_Com << endl;
			// cout << "ReLU_Com: "<< ReLU_Com << endl;
			// cout << "MP_Com: "<< MP_Com << endl;
			// cout << "MP_ReLU_Com: "<< MP_ReLU_Com << endl;
			// cout << "BN_Com: "<< BN_Com << endl;
			// cout << "Div_Com: "<< Div_Com << endl;
			myfile.open (filename.c_str(),fstream::app);
			myfile << "----------------------------------------------" << endl << endl; 
			
			myfile << "P" << partyNum << endl;
			myfile << "MatMul_Com: "<< (float)(MatMul_Com)/1000000 << endl;
			myfile << "ReLU_Com: "<< (float)(ReLU_Com)/1000000<<   endl;
			myfile << "MP_Com: "<< (float)(MP_Com)/1000000 << endl;
			myfile << "MP_ReLU_Com: "<< (float)(MP_ReLU_Com)/1000000 << endl;
			myfile << "BN_Com: "<< (float)(BN_Com)/1000000 << endl;
			myfile << "Div_Com: "<< (float)(Div_Com)/1000000 << endl;
			myfile << "PP_Com: "<< (float)(PP_Com)/1000000 << endl;
			myfile << "SS_Com: "<< (float)(SS_Com)/1000000 << endl;
			myfile << "----------------------------------------------" << endl << endl; 
			myfile << "P" << partyNum << endl;
			myfile << "MatMul_time: "<< (float)(MatMul_time)/CLOCKS_PER_SEC << endl;
			myfile << "ReLU_time: "<< (float)(ReLU_time)/CLOCKS_PER_SEC << endl;
			myfile << "MP_time: "<< (float)(MP_time)/CLOCKS_PER_SEC << endl;
			myfile << "MP_ReLU_time: "<< (float)(MP_ReLU_time)/CLOCKS_PER_SEC << endl;
			myfile << "BN_time: "<< (float)(BN_time)/CLOCKS_PER_SEC << endl;
			myfile << "Div_time: "<< (float)(Div_time)/CLOCKS_PER_SEC << endl;
			myfile << "----------------------------------------------" << endl << endl; 
			myfile << "P" << partyNum << endl;
			myfile << "MatMul_rounds: "<< MatMul_rounds << endl;
			myfile << "ReLU_rounds: "<< ReLU_rounds << endl;
			myfile << "MP_rounds: "<< MP_rounds << endl;
			myfile << "BN_rounds: "<< BN_rounds << endl;
			myfile << "Div_rounds: "<< Div_rounds << endl;
			myfile << "PP_rounds: "<< PP_rounds << endl;
			myfile << "SS_rounds: "<< SS_rounds << endl;
			myfile << "----------------------------------------------" << endl << endl; 




			myfile.close();



			printNetwork(net,filename1);

		/****************************** CLEAN-UP ******************************/ 
			delete aes_indep;
			delete aes_next;
			delete aes_prev;
			delete config;
			delete net;
			deleteObjects();
	}
	}

	return 0;
}




