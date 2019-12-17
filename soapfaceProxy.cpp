/* soapfaceProxy.cpp
   Generated by gSOAP 2.8.96 for face.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapfaceProxy.h"

faceProxy::faceProxy()
{	this->soap = soap_new();
	this->soap_own = true;
	faceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

faceProxy::faceProxy(const faceProxy& rhs)
{	this->soap = rhs.soap;
	this->soap_own = false;
	this->soap_endpoint = rhs.soap_endpoint;
}

faceProxy::faceProxy(struct soap *_soap)
{	this->soap = _soap;
	this->soap_own = false;
	faceProxy_init(_soap->imode, _soap->omode);
}

faceProxy::faceProxy(struct soap *_soap, const char *soap_endpoint_url)
{	this->soap = _soap;
	this->soap_own = false;
	faceProxy_init(_soap->imode, _soap->omode);
	soap_endpoint = soap_endpoint_url;
}

faceProxy::faceProxy(const char *soap_endpoint_url)
{	this->soap = soap_new();
	this->soap_own = true;
	faceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = soap_endpoint_url;
}

faceProxy::faceProxy(soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	faceProxy_init(iomode, iomode);
}

faceProxy::faceProxy(const char *soap_endpoint_url, soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	faceProxy_init(iomode, iomode);
	soap_endpoint = soap_endpoint_url;
}

faceProxy::faceProxy(soap_mode imode, soap_mode omode)
{	this->soap = soap_new();
	this->soap_own = true;
	faceProxy_init(imode, omode);
}

faceProxy::~faceProxy()
{	if (this->soap_own)
	{	this->destroy();
		soap_free(this->soap);
	}
}

void faceProxy::faceProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this->soap, imode);
	soap_omode(this->soap, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL },
        { "SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "ns", "http://tempuri.org/ns.xsd", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this->soap, namespaces);
}

faceProxy *faceProxy::copy()
{	faceProxy *dup = SOAP_NEW_UNMANAGED(faceProxy);
	if (dup)
	{	soap_done(dup->soap);
		soap_copy_context(dup->soap, this->soap);
	}
	return dup;
}

faceProxy& faceProxy::operator=(const faceProxy& rhs)
{	if (this->soap != rhs.soap)
	{	if (this->soap_own)
			soap_free(this->soap);
		this->soap = rhs.soap;
		this->soap_own = false;
		this->soap_endpoint = rhs.soap_endpoint;
	}
	return *this;
}

void faceProxy::destroy()
{	soap_destroy(this->soap);
	soap_end(this->soap);
}

void faceProxy::reset()
{	this->destroy();
	soap_done(this->soap);
	soap_initialize(this->soap);
	faceProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void faceProxy::soap_noheader()
{	this->soap->header = NULL;
}

::SOAP_ENV__Header *faceProxy::soap_header()
{	return this->soap->header;
}

::SOAP_ENV__Fault *faceProxy::soap_fault()
{	return this->soap->fault;
}

const char *faceProxy::soap_fault_subcode()
{	return ::soap_fault_subcode(this->soap);
}

const char *faceProxy::soap_fault_string()
{	return ::soap_fault_string(this->soap);
}

const char *faceProxy::soap_fault_detail()
{	return ::soap_fault_detail(this->soap);
}

int faceProxy::soap_close_socket()
{	return ::soap_closesock(this->soap);
}

int faceProxy::soap_force_close_socket()
{	return ::soap_force_closesock(this->soap);
}

void faceProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this->soap, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void faceProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this->soap, os);
}
#endif

char *faceProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this->soap, buf, len);
}
#endif

int faceProxy::send_FaceDetectByImgpath(const char *soap_endpoint_url, const char *soap_action, const std::string& img_path)
{
	struct ns__FaceDetectByImgpath soap_tmp_ns__FaceDetectByImgpath;
	if (soap_endpoint_url != NULL)
		soap_endpoint = soap_endpoint_url;
	soap_tmp_ns__FaceDetectByImgpath.img_path = img_path;
	soap_begin(soap);
	soap->encodingStyle = ""; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_ns__FaceDetectByImgpath(soap, &soap_tmp_ns__FaceDetectByImgpath);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__FaceDetectByImgpath(soap, &soap_tmp_ns__FaceDetectByImgpath, "ns:FaceDetectByImgpath", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__FaceDetectByImgpath(soap, &soap_tmp_ns__FaceDetectByImgpath, "ns:FaceDetectByImgpath", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int faceProxy::recv_FaceDetectByImgpath(struct FaceRes *face_res)
{
	struct ns__FaceDetectByImgpathResponse *soap_tmp_ns__FaceDetectByImgpathResponse;
	if (!face_res)
		return soap_closesock(soap);
	soap_default_FaceRes(soap, face_res);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__FaceDetectByImgpathResponse = soap_get_ns__FaceDetectByImgpathResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__FaceDetectByImgpathResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (face_res && soap_tmp_ns__FaceDetectByImgpathResponse->face_res)
		*face_res = *soap_tmp_ns__FaceDetectByImgpathResponse->face_res;
	return soap_closesock(soap);
}

int faceProxy::send_FaceDetectByBase64(const char *soap_endpoint_url, const char *soap_action, const std::string& img_base64)
{
	struct ns__FaceDetectByBase64 soap_tmp_ns__FaceDetectByBase64;
	if (soap_endpoint_url != NULL)
		soap_endpoint = soap_endpoint_url;
	soap_tmp_ns__FaceDetectByBase64.img_base64 = img_base64;
	soap_begin(soap);
	soap->encodingStyle = ""; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_ns__FaceDetectByBase64(soap, &soap_tmp_ns__FaceDetectByBase64);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__FaceDetectByBase64(soap, &soap_tmp_ns__FaceDetectByBase64, "ns:FaceDetectByBase64", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__FaceDetectByBase64(soap, &soap_tmp_ns__FaceDetectByBase64, "ns:FaceDetectByBase64", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int faceProxy::recv_FaceDetectByBase64(struct FaceRes *face_res)
{
	struct ns__FaceDetectByBase64Response *soap_tmp_ns__FaceDetectByBase64Response;
	if (!face_res)
		return soap_closesock(soap);
	soap_default_FaceRes(soap, face_res);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__FaceDetectByBase64Response = soap_get_ns__FaceDetectByBase64Response(soap, NULL, "", NULL);
	if (!soap_tmp_ns__FaceDetectByBase64Response || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (face_res && soap_tmp_ns__FaceDetectByBase64Response->face_res)
		*face_res = *soap_tmp_ns__FaceDetectByBase64Response->face_res;
	return soap_closesock(soap);
}
/* End of client proxy code */
