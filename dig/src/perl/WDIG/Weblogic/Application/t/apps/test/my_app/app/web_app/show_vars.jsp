<%@ page contentType="text/plain" %>
<%

  out.println("my_app.prop = "+System.getProperty("my_app.prop"));
  out.println("server = "+System.getProperty("server"));
  out.println("WDIG.Weblogic.ListenPort = "+System.getProperty("WDIG.Weblogic.ListenPort"));
  out.println("WDIG.Weblogic.Machine = "+System.getProperty("WDIG.Weblogic.Machine"));
  out.println("WDIG.Weblogic.FrontendHost = "+System.getProperty("WDIG.Weblogic.FrontendHost"));
  out.println("WDIG.Weblogic.FrontendHTTPPort = "+System.getProperty("WDIG.Weblogic.FrontendHTTPPort"));
  out.println("WDIG.Weblogic.Path = "+System.getProperty("WDIG.Weblogic.Path"));

%>
