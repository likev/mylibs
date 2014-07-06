
//SA SB�״������ ���ݽṹ
#ifndef _SADATASTRUCT_
#define _SADATASTRUCT_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

//DataBase.h
//SA��SB�״�ľ������
/** /
#define RGates  460				//S BAND �����ʾ������
#define VGates  920				//S BAND �ٶȾ������
#define WGates  920				//S BAND �׿�������
/ **/

//CB �״�ľ������
/*
#define RGates  800				//C BAND �����ʾ������
#define VGates  1600			//C BAND �ٶȾ������
#define WGates  1600			//C BAND �׿�������
*/



#define VALUE_INVALID -999.		//ʵ��ֵ�е�������,��ʾ����Ч�۲�����
#define VALUE_RANFOLD  999.		//ʵ��ֵ�е�������,��ʾ�о���ģ��
#define VALUE_AINVALID -1;

//////////////////////////////////////////////////////////////////////
//�״�����ݽṹ
template <unsigned RGates_VGates_WGates>
struct RadarBaseData
{
	unsigned short		temp1[7];						//����
	unsigned short		RadarStatus;					//1 - ��ʾΪ�״����� 
	unsigned short		temp2[6];						//����
	unsigned int		mSeconds;						//���������ռ�ʱ��(����,��00:00��ʼ)
	unsigned short		JulianDate;						//��1970/1/1�������
	unsigned short		URange;							//��ģ�����루��ʾ����ֵ/10.=ǧ�ף�
	unsigned short		Az;								//��λ�Ƕȣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	unsigned short		RadialNumber;					//�����������
	unsigned short		RadialStatus;					//��������״̬
	unsigned short		El;								//���ǣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	unsigned short		ElNumber;						//��ɨ�ڵ����Ǳ��
	short				RangeToFirstGateOfRef;			//��һ�����������ݱ�ʾ��ʵ�ʾ���(m)
	short				RangeToFirstGateOfDop;			//��һ�����������ݱ�ʾ��ʵ�ʾ���(m)
	unsigned short		GateSizeOfReflectivity;			//���������ݵľ���ⳤ(m)
	unsigned short		GateSizeOfDoppler;				//���������ݵľ���ⳤ(m)
	unsigned short		GatesNumberOfReflectivity;		//���������ݵľ������
	unsigned short		GatesNumberOfDoppler;			//���������ݵľ������
	unsigned short		CutSectorNumber; 				//������
	unsigned int		CalibrationConst; 				//�궨����
	unsigned short		PtrOfReflectivity;				//����������ָ��
	unsigned short		PtrOfVelocity;					//�ٶ�����ָ��
	unsigned short		PtrOfSpectrumWidth;				//�׿�����ָ��
	unsigned short		ResolutionOfVelocity;			//�������ٶȷֱ���:2=0.5m/s;4=1.0m/s
	unsigned short		VcpNumber;						//��ɨģʽ 11�� ��ˮģʽ��16������ 21����ˮģʽ��14������ 31�����ģʽ��8������ 32�����ģʽ��7������
	unsigned short		temp4[4];						//����
	unsigned short		PtrOfArcReflectivity;			//����������ָ��
	unsigned short		PtrOfArcVelocity;				//�ٶ�����ָ��
	unsigned short		PtrOfArcWidth;					//�׿�����ָ��
	unsigned short		Nyquist;						//��ģ���ٶȣ���ʾ����ֵ/100. = ��/�룩
	unsigned short      temp46;                         //����
	unsigned short      temp47;                         //����
	unsigned short      temp48;                         //����
	unsigned short      CircleTotal;					//������
	unsigned char		temp5[30];						//����
	unsigned char		Echodata[RGates_VGates_WGates];	//129��2428 ��2300�ֽ� (460�ֽڷ���������+920�ֽ��ٶ�����+920�ֽ��׿�����)
	unsigned char		temp[4];						//����

};

class RadarElevation{
public:
	enum CODE_MAEK{
		CODE_INVALID = 0,			//����ֵ�е�������,��ʾ����Ч�۲�����
		CODE_RANFOLD = 1          //����ֵ�е�������,��ʾ�о���ģ��
	};

	enum VELOCITY_RESOLUTION {
		RES_POINT_FIVE = 2,		//�ٶȾ�������,����ľ���Ϊ0.5 M/S
		RES_ONE_POINT = 4		//�ٶȾ�������,����ľ���Ϊ1.0 M/S
	};
public:
	std::vector<std::vector<double> > r, v, w;

	bool r_valid, v_valid, w_valid, 
		r_valid_this, v_valid_this, w_valid_this;

	unsigned r_pos, v_pos, w_pos;

	double Elevation;

	unsigned int r_gate_length, vw_gate_length, 
		r_gate_count, vw_gate_count; 

	double r_first_distance, vw_first_distance;

	int v_resolution;

	int date_begin, date_end, seconds_begin, seconds_end;
public:
	RadarElevation():
		r_valid(false), v_valid(false), w_valid(false)
	{
		
	}

	void set_r_gate_count(unsigned rcount)
	{
		r_gate_count = rcount;

		r.assign(360, std::vector<double>(rcount, VALUE_INVALID));
	}

	void set_vw_gate_count(unsigned rcount)
	{
		vw_gate_count = rcount;
		v.assign(360, std::vector<double>(rcount, VALUE_INVALID));
		w.assign(360, std::vector<double>(rcount, VALUE_INVALID));
	}

	bool push_back(unsigned az, unsigned char* pdata)
	{
		if(az >= 360) az -= 360;

		if(r_valid_this) 
		{
			push_back_r( az,  pdata);
		}
		if(v_valid_this) 
		{
			push_back_v( az, pdata);
		}
		if(w_valid_this) 
		{
			push_back_w( az, pdata);
		}

		return true;
	}


private:

	//* ���ͣ�ʵ�ͣ����ؽ����ķ����ʣ�DBZ����
	double decode_ref(unsigned char code)
	{
		if(code==CODE_INVALID)		return VALUE_INVALID;
		else if(code==CODE_RANFOLD)	return VALUE_RANFOLD;
		else						
			return (code-2.)/2.-32.5;
	}

	//* ���ͣ�ʵ�ͣ����ؽ����ľ����ٶȣ�M/S����
	float decode_vel(unsigned char code, short ResType)
	{
		if(code==CODE_INVALID)		return VALUE_INVALID;
		else if(code==CODE_RANFOLD)	return VALUE_RANFOLD;
		else						
		{
			if(ResType==RES_POINT_FIVE) //0.5 m/s 
				return (float((code-2.)/2.-63.5));
			else
				return (float((code-2)-127.));
		}
	}

	//* ���ͣ�ʵ�ͣ����ؽ������׿�M/S����
	float decode_spw(unsigned char code)
	{
		if(code==CODE_INVALID)		return VALUE_INVALID;
		else if(code==CODE_RANFOLD)	return VALUE_RANFOLD;
		else						
			return (float((code-2.)/2.-63.5));
	}
	bool push_back_r(unsigned az, unsigned char* pdata)
	{
		//Get first bin, last bin, and number of bins
		int FstBin = int(r_first_distance/r_gate_length+0.5);
		int BinNum = r_gate_count;
		if(FstBin<0)
		{
			BinNum = FstBin+BinNum;
			FstBin = -1*FstBin;
		}
		int LstBin = FstBin + BinNum;

		//Save data
		for(int BnIndex=FstBin; BnIndex<LstBin; BnIndex++)
		{
			r[az][BnIndex] = decode_ref(pdata[r_pos+BnIndex]);
		}

		return true;
	}
	bool push_back_v(unsigned az, unsigned char* pdata)
	{
		//Get first bin, last bin, and number of bins
		int FstBin = int(vw_first_distance/vw_gate_length+0.5);
		int BinNum = vw_gate_count;
		if(FstBin<0)
		{
			BinNum = FstBin+BinNum;
			FstBin = -1*FstBin;
		}
		int LstBin = FstBin + BinNum;

		//Save data
		for(int BnIndex=FstBin; BnIndex<LstBin; BnIndex++)
		{
			v[az][BnIndex] = decode_vel(pdata[v_pos+BnIndex], v_resolution);
		}
		return true;
	}
	bool push_back_w(unsigned az, unsigned char* pdata)
	{
		//Get first bin, last bin, and number of bins
		int FstBin = int(vw_first_distance/vw_gate_length+0.5);
		int BinNum = vw_gate_count;
		if(FstBin<0)
		{
			BinNum = FstBin+BinNum;
			FstBin = -1*FstBin;
		}
		int LstBin = FstBin + BinNum;

		//Save data
		for(int BnIndex=FstBin; BnIndex<LstBin; BnIndex++)
		{
			w[az][BnIndex] = decode_spw(pdata[w_pos+BnIndex]);
		}
		return true;
	}
};

class SA_SB_Info
{
public:
	typedef RadarBaseData<2300> SA_Base,SB_Base;//RGates+VGates+WGates
	enum RADIAL_STATUS {
		VOL_BEG=3,   //��ɨ��ʼ״̬��־
		VOL_END=4,	 //��ɨ����״̬��־
		ELV_BEG=0,	 //���ǿ�ʼ״̬��־
		ELV_END=2,   //���ǽ���״̬��־
		ELV_NORMAL=1
	};

	enum VOL_MODEL {
		RAIN16 = 11,  //��ˮģʽ��16������
		RAIN14 = 21,  //��ˮģʽ��14������
		SUN8 = 31,  // ���ģʽ��8������
		SUN7 = 32 //   ���ģʽ��7������
	};

public:
	std::vector<SB_Base> alldata;

	int date_begin, date_end, seconds_begin, seconds_end;

	class compare_by_double
	{
	public:
		bool operator() (const std::string &left, const std::string &right)const
		{
			double leftd, rightd;
			std::istringstream instr(left+' '+right);
			instr>>leftd>>rightd;

			return leftd < rightd;
		}
	};
	std::map<std::string, RadarElevation, compare_by_double> elevations;
	typedef std::map<std::string, RadarElevation, compare_by_double>::iterator r_it;
public:
	SA_SB_Info()
	{};
	SA_SB_Info(const std::string& filename)
	{
		read_base_data(filename);
	};

	bool read_base_data(const std::string& filename)
	{
		//"Z_RADR_I_Z9398_20120717041200_O_DOR_SB_CAP.bin"
		std::ifstream fin(filename.c_str(), std::ios_base::in|std::ios_base::binary);

		if(!fin.is_open())
		{
			std::cout<<"���ļ�"<<filename<<"ʧ�ܣ�"<<std::endl;
		}

		SB_Base oneline;
		alldata.clear();

		while(fin.read( (char *)(&oneline), sizeof(SB_Base)) )
		{
			alldata.push_back(oneline);
		}

		fin.close();

		init_elevations();//��ʼ���������ǲ� �洢��map��

		return true;
	}

	bool init_elevs()//���ⲿ���� //��ʼ���������ǲ�
	{
		return init_elevations();
	}

	const RadarElevation & operator() (const std::string &el)const
	{
		return elevations.find(el)->second;
	}

	

	void out_info(std::ostream &out)
	{
		out<<
			"��1970/1/1������� "
			"��������ʱ��  "
			"��ɨģʽ "
			"��ģ������  "
			"��� "
			"״̬  "
			"���Ǳ��    "
			"����    "
			"��λ��  "
			"��һ�����������ݱ�ʾ��ʵ�ʾ���(m) "
			"��һ�����������ݱ�ʾ��ʵ�ʾ���(m) "
			"���������ݵľ���ⳤ(m)  "
			"���������ݵľ���ⳤ(m)  "
			"���������ݵľ������   "
			"���������ݵľ������   "
			"����������ָ��  "
			"�ٶ�����ָ��   "
			"�׿�����ָ��   "
			"�������ٶȷֱ���  "
			"����������ָ��2  "
			"�ٶ�����ָ��2   "
			"�׿�����ָ��2   "
			"��ģ���ٶ�  "
			"������  "

			"\n"<<std::endl;
		for(unsigned i=0; i!=alldata.size(); i++)
		{
			SB_Base &cur = alldata[i];
			out<<cur.JulianDate<<"  "
				<<cur.mSeconds<<"   "
				<<get_vol_model(cur.VcpNumber)<<"  "
				<<cur.URange<<"  "
				<<cur.RadialNumber<<"  "
				<<get_radial_status(cur.RadialStatus)<<"   "
				<<cur.ElNumber<<"  "
				<<cur.El<<"  "
				<<cur.Az<<"  "
				<<cur.RangeToFirstGateOfRef<<"   "			//
				<<cur.RangeToFirstGateOfDop<<"   "
				<<cur.GateSizeOfReflectivity<<"   "
				<<cur.GateSizeOfDoppler<<"   "
				<<cur.GatesNumberOfReflectivity<<"   "
				<<cur.GatesNumberOfDoppler<<"  "
				<<cur.PtrOfReflectivity<<"  "
				<<cur.PtrOfVelocity<<"   "
				<<cur.PtrOfSpectrumWidth<<"  "
				<<cur.ResolutionOfVelocity<<"  "
				<<cur.PtrOfArcReflectivity<<"  "
				<<cur.PtrOfArcVelocity<<"   "
				<<cur.PtrOfArcWidth<<"  "
				<<cur.Nyquist<<"  "
				<<cur.CircleTotal<<"  "
				<<std::endl;
		}
	}

private:

	//������ת��ΪС�����һλ���ַ���
	std::string trans_to_el(double el)
	{
		std::ostringstream ostr;
		ostr.precision(1);
		ostr<<std::fixed<<el;

		return ostr.str();
	}
	//ת��Ϊʵ�ʵ�����
	double trans_angle(unsigned short angle)
	{
		return angle/8.0*180.0/4096.0;
	}


	//��ʼ���������ǲ� �洢��map��
	bool init_elevations()
	{
		std::string cur_el;
		double angle;

		//ѭ���������ǵĸ�������
		for(std::vector<SB_Base>::iterator it=alldata.begin(); it != alldata.end(); it++)
		{
			switch(it->RadialStatus)
			{
			case VOL_BEG:
				date_begin = it->JulianDate;
				seconds_begin = it->mSeconds;
			case ELV_BEG:  //���ǿ�ʼ
				angle = int(trans_angle(it->El)*10+0.5)/10.0;
				cur_el = trans_to_el(angle);

				elevations[cur_el].Elevation = angle;
				
				if(it->PtrOfReflectivity)
				{
					elevations[cur_el].date_begin = it->JulianDate;
					elevations[cur_el].seconds_begin = it->mSeconds;

					elevations[cur_el].set_r_gate_count(it->GatesNumberOfReflectivity);

					elevations[cur_el].r_gate_length = it->GateSizeOfReflectivity;
					elevations[cur_el].r_first_distance = it->RangeToFirstGateOfRef;

					elevations[cur_el].r_valid  = true;
					elevations[cur_el].r_pos = it->PtrOfReflectivity - 100;
				}
				if(it->PtrOfVelocity)
				{
					elevations[cur_el].set_vw_gate_count(it->GatesNumberOfDoppler);

					elevations[cur_el].vw_gate_length = it->GateSizeOfDoppler;
					elevations[cur_el].vw_first_distance = it->RangeToFirstGateOfDop;

					elevations[cur_el].v_valid  = true;
					elevations[cur_el].v_pos = it->PtrOfVelocity - 100;

					elevations[cur_el].v_resolution = it->ResolutionOfVelocity;
				}
				if(it->PtrOfSpectrumWidth)
				{
					elevations[cur_el].w_valid  = true;
					elevations[cur_el].w_pos = it->PtrOfSpectrumWidth - 100;
				}

				elevations[cur_el].r_valid_this  = it->PtrOfReflectivity;
				elevations[cur_el].v_valid_this  = it->PtrOfVelocity;
				elevations[cur_el].w_valid_this  = it->PtrOfSpectrumWidth;
				
				break;

			case VOL_END:
				date_end = it->JulianDate;
				seconds_end = it->mSeconds;

			case ELV_END:
				if(it->PtrOfReflectivity)
				{
					elevations[cur_el].date_end = it->JulianDate;
					elevations[cur_el].seconds_end = it->mSeconds;
				}
				  break;
			case ELV_NORMAL:
				break;
			default:
				;
			}
			//��ǰ���ǲ��ϼ��뱾�η�λ������
			elevations[cur_el].push_back(int(trans_angle(it->Az) +0.5), it->Echodata);
		}

		return true;
	}

	//��ȡ����״̬
	std::string get_radial_status(unsigned short RadialStatus)
	{
		std::string status;

		switch(RadialStatus)
		{
		case VOL_BEG:
			status = "VOL_BEG"; break;
		case VOL_END:
			status = "VOL_END"; break;
		case ELV_BEG:
			status = "ELV_BEG"; break;
		case ELV_END:
			status = "ELV_END"; break;
		case ELV_NORMAL:
			status = "Normal"; break;
		default:
			status = "UNKNOW";
		}

		return status;
	}
	//��ȡ��ɨģʽ
	std::string get_vol_model(unsigned short VcpNumber)
	{
		std::string status;

		switch(VcpNumber)
		{
		case RAIN16:
			status = "RAIN16"; break;
		case RAIN14:
			status = "RAIN14"; break;
		case SUN8:
			status = "SUN8"; break;
		case SUN7:
			status = "SUN7"; break;
		default:
			status = "UNKNOW";
		}

		return status;
	}
};

#endif