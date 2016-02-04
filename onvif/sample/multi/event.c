//create by liulu, 2016-1-20
//实现discover event，当发生对应的event时，会调用到对应的处理函数
#include "discoverH.h"
#include "../plugin/wsddapi.h"
#include <stdio.h>

void wsdd_event_Hello (struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here\n",__FILE__,__func__,__LINE__);
	printf("InstanceId %d\n",InstanceId);
	printf("SequenceID %s\n",SequenceId==NULL?"NULL":SequenceId);
	printf("MessageNumber %d\n",MessageNumber);
	printf("MessageID %s\n",MessageID==NULL?"NULL":MessageID);
	printf("RelatesTo %s\n",RelatesTo==NULL?"NULL":RelatesTo); //对应于发送的时填写的MessageID
	//必须全球唯一，且稳定
	printf("EndpointReference %s\n",EndpointReference==NULL?"NULL":EndpointReference);
	printf("Types %s\n",Types==NULL?"NULL":Types);
	printf("Scopes %s\n",Scopes==NULL?"NULL":Scopes);
	printf("MatchBy %s\n",MatchBy==NULL?"NULL":MatchBy);
	printf("XAddrs %s\n",XAddrs==NULL?"NULL":XAddrs);
	printf("MetadataVersion %d\n",MetadataVersion);
	printf("\033[0m");
	printf("===========================================\n");

}
void wsdd_event_Bye (struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here\n",__FILE__,__func__,__LINE__);
	printf("\033[0m");
	printf("===========================================\n");
}
soap_wsdd_mode wsdd_event_Probe (struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *matches)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here\n",__FILE__,__func__,__LINE__);
	//TODO:回送probeMatches

	printf("MessageID %s\n",MessageID==NULL?"NULL":MessageID);
	printf("ReplyTo %s\n",ReplyTo==NULL?"NULL":ReplyTo); //对应于发送的时填写的MessageID
	printf("Types %s\n",Types==NULL?"NULL":Types);
	printf("Scopes %s\n",Scopes==NULL?"NULL":Scopes);
	printf("MatchBy %s\n",MatchBy==NULL?"NULL":MatchBy);

	if(matches!=NULL)
	{
		int i;
		printf("matches count:%d\n",matches->__sizeProbeMatch);
		for( i = 0;i<matches->__sizeProbeMatch;i++)
		{
			printf("Endpoint Address:%s\n",matches->ProbeMatch[i].wsa__EndpointReference.Address==NULL?"NULL":matches->ProbeMatch[i].wsa__EndpointReference.Address);
			printf("Types:%s\n",matches->ProbeMatch[i].Types==NULL?"NULL":matches->ProbeMatch[i].Types);
			if(matches->ProbeMatch[i].Scopes!=NULL)
			{
				printf("Scopes item:%s\n",matches->ProbeMatch[i].Scopes->__item==NULL?"NULL":matches->ProbeMatch[i].Scopes->__item);
				printf("Scopes MatchBy:%s\n",matches->ProbeMatch[i].Scopes->MatchBy==NULL?"NULL":matches->ProbeMatch[i].Scopes->MatchBy);
			}
			else
			{
				printf("Scopes is NULL\n");
			}
			printf("XAddr:%s\n",matches->ProbeMatch[i].XAddrs==NULL?"NULL":matches->ProbeMatch[i].XAddrs);
			printf("MetadataVersion:%d\n",matches->ProbeMatch[i].MetadataVersion);	
		}
	}
	else
	{
		printf("matches is NULL\n");
	}
	printf("\033[0m");
	printf("===========================================\n");
	return SOAP_WSDD_ADHOC;
}
void wsdd_event_ProbeMatches (struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *matches)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here,start\n",__FILE__,__func__,__LINE__);
	printf("InstanceId %d\n",InstanceId);
	printf("SequenceID %s\n",SequenceId==NULL?"NULL":SequenceId);
	printf("MessageNumber %d\n",MessageNumber);
	printf("MessageID %s\n",MessageID==NULL?"NULL":MessageID);
	printf("RelatesTo %s\n",RelatesTo==NULL?"NULL":RelatesTo); //对应于发送的时填写的MessageID
	if(matches!=NULL)
	{
		int i;
		printf("matches count:%d\n",matches->__sizeProbeMatch);
		for( i = 0;i<matches->__sizeProbeMatch;i++)
		{
			printf("Endpoint Address:%s\n",matches->ProbeMatch[i].wsa__EndpointReference.Address==NULL?"NULL":matches->ProbeMatch[i].wsa__EndpointReference.Address);
			printf("Types:%s\n",matches->ProbeMatch[i].Types==NULL?"NULL":matches->ProbeMatch[i].Types);
			if(matches->ProbeMatch[i].Scopes!=NULL)
			{
				printf("Scopes item:%s\n",matches->ProbeMatch[i].Scopes->__item==NULL?"NULL":matches->ProbeMatch[i].Scopes->__item);
				printf("Scopes MatchBy:%s\n",matches->ProbeMatch[i].Scopes->MatchBy==NULL?"NULL":matches->ProbeMatch[i].Scopes->MatchBy);
			}
			else
			{
				printf("Scopes is NULL\n");
			}
			printf("XAddr:%s\n",matches->ProbeMatch[i].XAddrs==NULL?"NULL":matches->ProbeMatch[i].XAddrs);
			printf("MetadataVersion:%d\n",matches->ProbeMatch[i].MetadataVersion);	
		}
	}
	else
	{
		printf("matches is NULL\n");
	}
	printf("file:%s func:%s line:%d at here,end\n",__FILE__,__func__,__LINE__);
	printf("\033[0m");	
	printf("===========================================\n");
}
soap_wsdd_mode wsdd_event_Resolve (struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here\n",__FILE__,__func__,__LINE__);
	printf("\033[0m");
	printf("===========================================\n");
	return SOAP_WSDD_ADHOC;
}
void wsdd_event_ResolveMatches (struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)
{
	printf("===========================================\n");
	printf("\033[34m");  //set out print font color is blue
	printf("file:%s func:%s line:%d at here\n",__FILE__,__func__,__LINE__);
	printf("\033[0m");
	printf("===========================================\n");
}







