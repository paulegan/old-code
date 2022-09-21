/**
 * A quick & simple implementation of an XML RPC servlet.
 * 
 * @author     paul.x.egan.-nd@email.disney.com
 * @version    20030813
 * @see        org.apache.xmlrpc.XmlRpcServer
 *
 */

package com.wdig.workflow.xmlrpc;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.util.Vector;
import org.apache.xmlrpc.XmlRpcServer;

public class RPCServlet extends HttpServlet
{

 private XmlRpcServer server;

 public void init(ServletConfig config)
        throws ServletException
 {

  server = new XmlRpcServer();

  try {
    server.addHandler("pluto", new RPCHandler());
  } catch (Exception x) {
    throw new ServletException("Unable to add handler: "+x.toString());
  }

 }

 public void doPost(HttpServletRequest request, HttpServletResponse response)
        throws ServletException, IOException
 {
  byte[] result = server.execute(request.getInputStream());
  response.setContentType("text/xml");
  response.setContentLength(result.length);
  OutputStream output = response.getOutputStream();
  output.write(result);
  output.flush();
 }

}
