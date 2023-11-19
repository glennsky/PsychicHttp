#include "PsychicHTTP.h"

PsychicHTTPServer::PsychicHTTPServer()
{
  //some configs
  this->config = HTTPD_DEFAULT_CONFIG();

  this->config.global_user_ctx = this;
  this->config.global_user_ctx_free_fn = this->destroy;

  //this->defaultEndpoint(this, HTTP_GET);
}

PsychicHTTPServer::~PsychicHTTPServer()
{

}

void PsychicHTTPServer::destroy(void *ctx)
{
  DUMP(ctx);
  PsychicHTTPServer *temp = (PsychicHTTPServer *)ctx;
  delete temp;
}

bool PsychicHTTPServer::begin(uint16_t port)
{
  this->config.server_port = port;

  /* Start the httpd server */
  if (httpd_start(&this->server, &this->config) != ESP_OK) {
    return true;
    // /* Register URI handlers */
    // httpd_register_uri_handler(server, &uri_get);
    // httpd_register_uri_handler(server, &uri_post);
  }

  return false;
}

bool PsychicHTTPServer::begin(uint16_t port, const char *cert, const char *private_key)
{
  this->config.server_port = port;

  /* Start the httpd server */
  if (httpd_start(&this->server, &this->config) == ESP_OK) {
    return true;
      // /* Register URI handlers */
      // httpd_register_uri_handler(server, &uri_get);
      // httpd_register_uri_handler(server, &uri_post);
  }

  return false;
}

// PsychicHTTPServerEndpoint::PsychicHTTPServerEndpoint(PsychicHTTPServer *server, httpd_method_t method)
// {
//   this->server = server;
//   this->method = method;
// }

// PsychicHTTPServerEndpoint::~PsychicHTTPServerEndpoint()
// {

// }

PsychicHTTPServerEndpoint *PsychicHTTPServer::on(const char* uri)
{
  return on(uri, HTTP_GET);
}

PsychicHTTPServerEndpoint *PsychicHTTPServer::on(const char* uri, PsychicHTTPRequestHandler onRequest)
{
  return on(uri, HTTP_GET)->onRequest(onRequest);
}

PsychicHTTPServerEndpoint *PsychicHTTPServer::on(const char* uri, httpd_method_t method, PsychicHTTPRequestHandler onRequest)
{
  return on(uri, method)->onRequest(onRequest);
}

PsychicHTTPServerEndpoint *PsychicHTTPServer::on(const char* uri, httpd_method_t method)
{
  PsychicHTTPServerEndpoint *handler = new PsychicHTTPServerEndpoint(this, method);

  // URI handler structure
  httpd_uri_t my_uri {
    .uri      = uri,
    .method   = method,
    .handler  = handler->endpointRequestHandler,
    .user_ctx = handler
  };

  // Register handler
  if (httpd_register_uri_handler(this->server, &my_uri) != ESP_OK) {
    Serial.println("Handler failed");
  }  
}

void PsychicHTTPServer::onNotFound(PsychicHTTPRequestHandler fn)
{
  defaultEndpoint.onRequest(fn);
}

void PsychicHTTPServer::eventHandler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
//   //what kind of event is it?
//   switch (ev)
//   {
//     // Connection accepted
//     case MG_EV_ACCEPT:
//       //Serial.println("MG_EV_ACCEPT");
//       char addr[32];
//       mg_snprintf(addr, sizeof(addr), "%M", mg_print_ip, &c->rem);                          
//       DBUGF("Connection %p from %s", c, addr);
//       break;

//     // TLS handshake succeeded
//     case MG_EV_TLS_HS:
//       Serial.println("MG_EV_TLS_HS");
//       break;

//     // Data received from socket - long *bytes_read
//     // case MG_EV_READ: {
//     //   Serial.println("MG_EV_READ");
//     //   int *num_bytes = (int *)ev_data;
//     //   DBUGF("Received %d bytes", *num_bytes);
//     //   break;
//     // }

//     // Data written to socket - long *bytes_written
//     // case MG_EV_WRITE:
//     //   Serial.println("MG_EV_WRITE");
//     //   //TODO: old code might fit here?
//     //   //     case MG_EV_POLL:
//     //   //     case MG_EV_SEND:
//     //   //     {
//     //   //       if(nc->user_data)
//     //   //       {
//     //   //         PsychicHTTPServerRequest *request = (PsychicHTTPServerRequest *)nc->user_data;
//     //   //         if(request->responseSent()) {
//     //   //           request->sendBody();
//     //   //         }
//     //   //       }
//     //   //       break;
//     //   //     }
//     //   break;

//     // Connection closed
//     case MG_EV_CLOSE:
//       DBUGF("Connection %p closed", c);
//       // TODO: we will need to handle this for websockets
//       // if(c->fn_data) 
//       // {
//       //   PsychicHTTPServerRequest *request = (PsychicHTTPServerRequest *)c->fn_data;
//       //   // TODO: calls a member, but we're an external function 
//       //   // if(endpoint->close) {
//       //   //   endpoint->close(request);
//       //   // }
//       //   delete request;
//       //   c->fn_data = NULL;
//       // } 
//       break;

//       // //did we match our websocket endpoint?
//       // if (mg_http_match_uri(hm, "/ws")) {
//       //   // Upgrade to websocket. From now on, a connection is a full-duplex
//       //   // Websocket connection, which will receive MG_EV_WS_MSG events.
//       //   mg_ws_upgrade(c, hm, NULL);
//       // }

//       //custom handler for not found.
//       if (!found)
//       {
//         PsychicHTTPServerRequest* request = new PsychicHTTPServerRequest(self, c, hm);
//         self->defaultEndpoint.request(request);
//       }

//       break;
//     }

//     // Websocket handshake done     struct mg_http_message *
//     case MG_EV_WS_OPEN: {
//       Serial.println("MG_EV_WS_OPEN");
//       hm = (struct mg_http_message *) ev_data;

//       //TODO: update this
//       // if(endpoint->wsConnect && nc->flags & MG_F_IS_PsychicHTTPWebSocketConnection)
//       // {
//       //   PsychicHTTPWebSocketConnection *c = (PsychicHTTPWebSocketConnection *)nc->user_data;
//       //   endpoint->wsConnect(c);
//       // }

//       break;
//     }

//     // Websocket msg, text or bin   struct mg_ws_message *
//     case MG_EV_WS_MSG: {
//       Serial.println("MG_EV_WS_MSG");
//       // Got websocket frame. Received data is wm->data. Echo it back!
//       struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
// //       if(endpoint->wsFrame && nc->flags & MG_F_IS_PsychicHTTPWebSocketConnection)
// //       {
// //         PsychicHTTPWebSocketConnection *c = (PsychicHTTPWebSocketConnection *)nc->user_data;
// //         struct websocket_message *wm = (struct websocket_message *)p;
// //         endpoint->wsFrame(c, wm->flags, wm->data, wm->size);
// //       }
//       break;
//     }

//     // Websocket control msg        struct mg_ws_message *
//     case MG_EV_WS_CTL: {
//       Serial.println("MG_EV_WS_CTL");
//       struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
//       break;
//     }
// //}
}

void PsychicHTTPServer::sendAll(PsychicHTTPWebSocketConnection *from, const char *endpoint, int op, const void *data, size_t len)
{
  // mg_mgr *mgr = Mongoose.getMgr();

  // const struct mg_connection *nc = from ? from->getConnection() : NULL;
  // struct mg_connection *c;
  // for (c = mgr->conns; c != NULL; c = c->next) {
  //   if (c == nc) { 
  //     continue; /* Don't send to the sender. */
  //   }
  //   if (c->is_websocket)
  //   {
  //     PsychicHTTPWebSocketConnection *to = (PsychicHTTPWebSocketConnection *)c->fn_data;
  //     if(endpoint && !to->uri().equals(endpoint)) {
  //       continue;
  //     }
  //     DBUGF("%.*s sending to %p", to->uri().length(), to->uri().c_str(), to);
  //     to->send(op, data, len);
  //   }
  // }
}

PsychicHTTPServerRequest::PsychicHTTPServerRequest(PsychicHTTPServer *server, httpd_req_t *req) :
  _server(server),
  _req(req),
  _response(NULL)
{
  // if(0 == mg_vcasecmp(&msg->method, "GET")) {
  //   _method = HTTP_GET;
  // } else if(0 == mg_vcasecmp(&msg->method, "POST")) {
  //   _method = HTTP_POST;
  // } else if(0 == mg_vcasecmp(&msg->method, "DELETE")) {
  //   _method = HTTP_DELETE;
  // } else if(0 == mg_vcasecmp(&msg->method, "PUT")) {
  //   _method = HTTP_PUT;
  // } else if(0 == mg_vcasecmp(&msg->method, "PATCH")) {
  //   _method = HTTP_PATCH;
  // } else if(0 == mg_vcasecmp(&msg->method, "HEAD")) {
  //   _method = HTTP_HEAD;
  // } else if(0 == mg_vcasecmp(&msg->method, "OPTIONS")) {
  //   _method = HTTP_OPTIONS;
  // }
}

PsychicHTTPServerRequest::~PsychicHTTPServerRequest()
{
  if(_response) {
    delete _response;
    _response = NULL;
  }
}

void PsychicHTTPServerRequest::redirect(const char *url)
{

}

PsychicHTTPServerResponse *PsychicHTTPServerRequest::beginResponse()
{
  return new PsychicHTTPServerResponse();
}

void PsychicHTTPServerRequest::send(PsychicHTTPServerResponse *response)
{
  response->send(this->_nc);
}

//what is this?
void PsychicHTTPServerRequest::send(int code)
{
  send(code, "text/plain", http_status_reason(code));
}

void PsychicHTTPServerRequest::send(int code, const char *contentType, const char *content)
{
  PsychicHTTPServerResponse *response = this->beginResponse();

  response->setCode(code);
  response->setContentType(contentType);
  response->setContent(content);
  
  this->send(response);
}

bool PsychicHTTPServerRequest::hasParam(const char *name) const
{
  char dst[8];
  int ret = getParam(name, dst, sizeof(dst));
  return ret >= 0 || -3 == ret; 
}

int PsychicHTTPServerRequest::getParam(const char *name, char *dst, size_t dst_len) const
{
  return mg_http_get_var((HTTP_GET == _method) ? (&_msg->query) : (&_msg->body), name, dst, dst_len);
}

bool PsychicHTTPServerRequest::authenticate(const char * username, const char * password)
{
  // DBUGVAR(username);
  // DBUGVAR(password);

  // char user_buf[64];
  // char pass_buf[64];

  // mg_http_creds(_msg, user_buf, sizeof(user_buf), pass_buf, sizeof(pass_buf));

  // DBUGVAR(user_buf);
  // DBUGVAR(pass_buf);

  // if(0 == strcmp(username, user_buf) && 0 == strcmp(password, pass_buf))
  // {
  //   return true;
  // }

  // return false;
}

void PsychicHTTPServerRequest::requestAuthentication(const char* realm)
{
  // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/WebRequest.cpp#L852
  // mg_http_send_digest_auth_request

  // char headers[64];
  // mg_snprintf(headers, sizeof(headers), 
  //     "WWW-Authenticate: Basic realm=%s",
  //     realm);

  // mg_http_reply(_nc, 401, headers, "", NULL);
}



PsychicHTTPServerResponse::PsychicHTTPServerResponse() :
  _code(200)
{

}

PsychicHTTPServerResponse::~PsychicHTTPServerResponse()
{
}

void PsychicHTTPServerResponse::addHeader(const char *name, const char *value)
{
  // mg_http_header header;
  // header.name = mg_str(name);
  // header.value = mg_str(value);
  // headers.push_back(header);
}

void PsychicHTTPServerResponse::setContentType(const char *contentType)
{
  addHeader("Content-Type", contentType);
}

void PsychicHTTPServerResponse::setContent(const char *content)
{
  this->body = content;
  setContentLength(strlen(content));
}

void PsychicHTTPServerResponse::setContent(const uint8_t *content, size_t len)
{
  this->body = (char *)content;
  setContentLength(len);
}

const char * PsychicHTTPServerResponse::getHeaderString()
{
  std::string h = "";

  for (mg_http_header header : this->headers)
  {
    h.append(header.name.ptr, header.name.len);
    h += ": ";
    h.append(header.value.ptr, header.value.len);
    h += "\r\n";
  }

  return h.c_str();
}

void PsychicHTTPServerResponse::send(struct mg_connection *nc)
{
  DUMP(_contentLength);
  DUMP(heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));

  if (false || _contentLength > heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT))
  {
    char temp[MG_IO_SIZE+1];
    temp[MG_IO_SIZE] = 0;
    size_t idx = 0;
    size_t remaining;

    mg_printf(nc, "HTTP/1.1 %u %s\r\n", _code, http_status_reason(_code));
    mg_printf(nc, getHeaderString());
    mg_printf(nc, "Content-Length: %u\r\n\r\n", _contentLength);

    for (idx = 0; idx<_contentLength; idx += MG_IO_SIZE)
    {
      remaining = _contentLength - idx;
      DUMP(remaining);
      if (remaining > MG_IO_SIZE)
      {
        strncpy(temp, body+idx, MG_IO_SIZE);
        temp[MG_IO_SIZE] = 0;
        mg_send(nc, temp, MG_IO_SIZE);
      }
      else
      {
        strncpy(temp, body+idx, remaining);
        temp[remaining] = 0;
        mg_send(nc, temp, remaining);
      }

      write_conn(nc);
    }
    
    nc->is_resp = 0;
    nc->is_draining = 1;
  }
  else
  {
    //mg_http_reply(nc, _code, getHeaderString(), body);
    mg_printf(nc, "HTTP/1.1 %u %s\r\n", _code, http_status_reason(_code));
    mg_printf(nc, getHeaderString());
    mg_printf(nc, "Content-Length: %u\r\n\r\n", _contentLength);
    mg_send(nc, body, _contentLength);
  }
}

// #ifdef ARDUINO
// PsychicHTTPServerResponseStream::PsychicHTTPServerResponseStream()
// {
//   mg_iobuf_init(&_content, ARDUINO_MONGOOSE_DEFAULT_STREAM_BUFFER, ARDUINO_MONGOOSE_DEFAULT_STREAM_BUFFER);
// }

// PsychicHTTPServerResponseStream::~PsychicHTTPServerResponseStream()
// {
//   mg_iobuf_free(&_content);
// }

// size_t PsychicHTTPServerResponseStream::write(const uint8_t *data, size_t len)
// {
//   size_t written = mg_iobuf_add(&_content, _content.len, data, len);
//   setContentLength(_content.len);
//   return written;
// }

// size_t PsychicHTTPServerResponseStream::write(uint8_t data)
// {
//   return write(&data, 1);
// }

// void PsychicHTTPServerResponseStream::send(struct mg_connection *nc)
// {
//   const char *headers = getHeaderString();

//   mg_http_reply(nc, _code, headers, body);
// }

// #endif

PsychicHTTPWebSocketConnection::~PsychicHTTPWebSocketConnection()
{

}

void PsychicHTTPWebSocketConnection::send(int op, const void *data, size_t len)
{
  //mg_ws_send(_nc, data, len, op);
}