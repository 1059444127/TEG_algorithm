
//���ļ���Ҫ��˵��TEG��λ��Ӧ���������������ݣ�
//��Ҫ���������ݵ�ľ�ֵ�˲�������������ȡ����Ҫ��������ȡ

//�洢�˲��������
QList<QPointF> List_Filter_Data��

//�洢��Ѫ������������֧����
QList<QPointF> List_Max_Curve_Data��

//�洢��Ѫ������������֧����
QList<QPointF> List_Max_Curve_Data��

/*
@���ݵ������ֵ�˲�������λ������5ms����һ��������
@��Σ�orign_data,���ڲ�����ԭʼ����
@      size,�������ݴ�С
@���Σ���
@����ֵ����
*/
void Smooth_Origin_Data(double* orign_data, unsigned size)
{
	//���������ԭʼ���ݴ�С����36�������������������������ڴ�֮ǰ������ж��Ƿ�Ϊ36��������
	if(size%36!=0) return;
	
	//���þ�ֵ�˲���Nֵѡ��Ϊ36����Ϊ���õ�һ��С����������36����
	for(unsigned i = 0; i < size; i+=36)
	{
		int sum = 0;
		int ave = 0;
		unsigned int j;
		for(j = i; j< i+36; j++){

			sum += data[j];
		}
		ave = sum/36;
		
		//��Ҫ�Ǵ洢�˲�������ݵ㣬ѡ����ֵ��Ϊ�˲�������ݵ�
		QPointF s(double(j+18+List_Filter_Data.size())*0.005,ave);	
		List_Filter_Data.append(s);
	}	
}

/*
@��Ѫ���ߵ�������ȡ
@��Σ�List_Filter_Data���˲��������
@���Σ���
@����ֵ����
*/
void ExtractionContour(QList<QPointF> List_Filter_Data)
{
	unsigned int size = List_Filter_Data.size();
	//����TEG��תһ������Ϊ10s���ң��˲����һ�����Ҳ�����ҲΪ10s�� 10000/36 = 55.5����ѡ��55����Ϊһ�������������
	for(unsigned int i = 0; i < size-55,i=i+55)
	{
		int xmax = 0;
		int xmin = 0;
		int ymax = 0;
		int ymin = 5000;

		for(unsigned int j = i;j<i-55;j++){
			if(List_Filter_Data.at(j).ry()> ymax){
				ymax = List_Filter_Data.at(j).ry();
				xmax = j-1;
			}
			if(List_Filter_Data.at(j).ry()< ymin){
				ymin = List_Filter_Data.at(j).ry();
				xmin = j-1;
			}
		
		QPointF max_point(xmax,ymax);
		QPointF min_point(xmin,ymin);
		List_Max_Curve_Data.append(max_point)��
		List_Min_Curve_Data.append(min_point)��
		
	}	
}
 
 
/*
@��ȡ��Ҫ����Ѫ������
@��Σ�List_Max_Curve_Data��������֧����
@      List_Min_Curve_Data��������ֵ����
@���Σ�R_Point,Rֵ�����
@	   K_Point,Kֵ�����
@      Angle_Point,angle�Ƕ�ֵ�����
@      MA_Point,MAֵ�����
@����ֵ����
*/

void GetMainParameter(QList<QPointF> List_Max_Curve_Data,QList<QPointF> List_Min_Curve_Data��QPointF &R_Point,QPointF &K_Point,QPointF &Angle_Point,QPointF &MA_Point)
{
	MA_Point = QPointF(0,5000);
	unsigned int size = List_Max_Curve_Data.size();
	for(unsigned int i = 0; i < size; ++i)
	{
		//Rֵ��ʾ��TEG�񵴷�ֵ����20mmΪRֵ�㡣
		if(List_Max_Curve_Data[i]>=(List_Max_Curve_Data[i]+List_Min_Curve_Data[i])/2 + 20){
			//Kֵ��ʾTEG�񵴷�ֵ����500�ĵ�
			if(List_Max_Curve_Data[i]<=(List_Max_Curve_Data[i]+List_Min_Curve_Data[i])/2 + 500){
				K_Point = List_Max_Curve_Data[i];
			}
			
			//����MAֵ,
			if(List_Min_Curve_Data[i].ry() < MA_Point.ry()){
				MA_Point = List_Min_Curve_Data[i];
			}			
			
			//��angle������
			lineParaPoint = linefunc(RvaluePoint,List_Max_Curve_Data[i]);
			if((List_Max_Curve_Data[i-2].y() < getlineY(lineParaPoint,List_Max_Curve_Data[i-2].x()))
				&&((List_Max_Curve_Data[i-3].y() < getlineY(lineParaPoint,List_Max_Curve_Data[i-3].x())))){
				AnglevaluePoint = List_Max_Curve_Data[i-1];
			}
			
		}else{
			//����񵴷�ֵ������20mm������滮Ϊһ��ƽ���ߣ������������ֵҲ���Ա�ʾ�񵴵�ʵʱֵ��
			List_Max_Curve_Data.append(QPointF(List_Max_Curve_Data[i].rx(),List_Max_Curve_Data[i]+List_Min_Curve_Data[i])/2);
			List_Max_Curve_Data.append(QPointF(List_Min_Curve_Data[i].rx(),List_Min_Curve_Data[i]+List_Min_Curve_Data[i])/2);
			R_Point = List_Max_Curve_Data[i];
		}
	}			
}


/*
@��ȡ�����б��k�ͽؾ�b ��y = kx+b
@��Σ�point1,��һ����
	   point2,�ڶ�����
@���Σ���
@����ֵ��QPointF(k,b)
*/
QPointF linefunc(QPointF point1, QPointF point2)
{
    QPointF linepara;
    linepara.setX((point1.ry()-point2.ry())/(point1.rx()-point2.rx()));
    linepara.setY(point1.ry()-(linepara.rx()*point1.rx()));
    return linepara;
}

/*
@��ȡ���ߵ�Y��ֵ
@��Σ�lineSlope����ʾ���ߵ�б�ʺͽؾ�
	   xValue,X���ֵ
@���Σ���
@����ֵ��Y���ֵ
*/
double getlineY(QPointF lineSlope, double xValue)
{
    double  yValue;
    yValue = (lineSlope.rx()*xValue)+lineSlope.ry();
    return yValue;
}


/*
@����Rֵ
@��Σ� R_Point Rֵ�����
@���Σ� strRvalue��Rֵ���ַ���ֵ
����ֵ����
*/
void SetRvalue(QPointF R_Point,QString &strRvalue)
{

    double value = R_Point.rx();
    QString strRvalue = QString::number(value/60,'f',2);
}


/*
@����Kֵ
@��Σ� K_Point Kֵ�����
@���Σ� strKvalue��Kֵ���ַ���ֵ
����ֵ����
*/
void SetRvalue(QPointF K_Point,QString &strKvalue)
{
    double value = K_Point.rx();
    QString strKvalue = QString::number(value/60,'f',2);
}


/*
@����MAֵ
@��Σ� MA_Point MAֵ�����
        baselineValue ���ߵ�y��ֵ
@���Σ� strMAvalue,MAֵ���ַ���ֵ
����ֵ����
*/
void SetRvalue(QPointF MA_Point,double baselineValue,QString &strMAvalue)
{
	//MAֵ�����������°�֧�����֮��*0.047
    double value = MA_Point.ry();
    QString strMAvalue = QString::number((baselineValue-value)*0.047,'f',2);
}

/*
@����ANGLEֵ
@��Σ� ANGLE_Point ANGLEֵ�����
        lineParaPoint ���ߵ�б�ʺͽؾ��
@���Σ� strAnglevalue,MAֵ���ַ���ֵ
����ֵ����
*/
void SetAnglevalue(QPointF AnglevaluePoint, QPointF lineParaPoint,,QString &strAnglevalue)
{
	//angle�Ƕ�ֵ���е���Rֵ���б�ʡ�k=((yangle-yRvalue)*0.047)/((xangle-xRvalue)*4/60)
    double tanvalue = ((AnglevaluePoint.ry()-preRValuePoint[channel].ry())*0.047)/((AnglevaluePoint.rx()-preRValuePoint[channel].rx())*4/60);
    double arctanvalue = (180*qAtan(tanvalue))/M_PI;
    QString strAnglevalue = QString::number(arctanvalue,'f',2);
}














