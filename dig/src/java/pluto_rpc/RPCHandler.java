/**
 * Exports certain methods through XML RPC
 * 
 * @author     paul.x.egan.-nd@email.disney.com
 * @version    20030813
 * @see        org.apache.xmlrpc.XmlRpcHandler
 *
 */

package com.wdig.workflow.xmlrpc;

import java.util.Vector;
import java.util.Hashtable;
import java.util.Iterator;

import org.apache.xmlrpc.XmlRpcHandler;
import org.apache.xmlrpc.XmlRpcException;

import com.wdig.workflow.common.UserData;
import com.wdig.workflow.ui.UserUseBean;
import com.wdig.workflow.common.ProjectData;
import com.wdig.workflow.ui.ProjectUseBean;
import com.wdig.workflow.common.DropData;
import com.wdig.workflow.common.DropStatus;
import com.wdig.workflow.common.FileData;
import com.wdig.workflow.ui.DropUseBean;
import com.wdig.workflow.common.UserAlertData;
import com.wdig.workflow.ui.UserAlertUseBean;
import com.wdig.workflow.common.Frequency;


public class RPCHandler implements XmlRpcHandler
{

 public Object execute(String method, Vector parameters)
        throws XmlRpcException
 {

   if ( "getUserData".equals(method) ) {
     return getUserData(parameters);
   } else if ( "getProjectData".equals(method) ) {
     return getProjectData(parameters);
   } else if ( "getDropData".equals(method) ) {
     return getDropData(parameters);
   } else if ( "getLastDrop".equals(method) ) {
     return getLastDrop(parameters);
   } else if ( "createUserAlert".equals(method) ) {
     return createUserAlert(parameters);
   } else {
     throw new XmlRpcException(0, "Unknown method '"+method+"'");
   }

 }

 private Hashtable getProjectData(Vector parameters)
        throws XmlRpcException
 {
   if ( parameters.size() < 1 ) {
     throw new XmlRpcException(1, "No project ID specified");
   }

   int projectId;
   try {
     projectId = ((Integer)parameters.firstElement()).intValue();
   } catch (Exception x) {
     throw new XmlRpcException(2, "Invalid ID number: "+parameters.firstElement().toString());
   }

   ProjectData projectData;
   try {
     ProjectUseBean projectUseBean = new ProjectUseBean();
     projectData = projectUseBean.getProjectData(projectId);
   } catch (Exception x) {
     throw new XmlRpcException(2, "Unable to get project data: "+x.toString());
   }

   if ( projectData == null )
     throw new XmlRpcException(2, "Project not found");

   return projectData2Hashtable(projectData);
 }


 private Hashtable getDropData(Vector parameters)
        throws XmlRpcException
 {
   if ( parameters.size() < 2 ) {
     throw new XmlRpcException(1, "No project ID & drop version specified");
   }

   int projectId, version;
   try {
     projectId = ((Integer)parameters.get(0)).intValue();
     version = ((Integer)parameters.get(1)).intValue();
   } catch (Exception x) {
     throw new XmlRpcException(2, "Invalid project ID or drop version: "+parameters.firstElement().toString());
   }

   DropData dropData;
   try {
     DropUseBean dropUseBean = new DropUseBean();
     dropData = dropUseBean.getDropData(projectId, version);
   } catch (Exception x) {
     throw new XmlRpcException(2, "Unable to get drop data: "+x.toString());
   }

   if ( dropData == null )
     throw new XmlRpcException(2, "Drop not found");

   return dropData2Hashtable(dropData);
 }


 private Hashtable getLastDrop(Vector parameters)
        throws XmlRpcException
 {
   if ( parameters.size() < 1 ) {
     throw new XmlRpcException(1, "No project ID specified");
   }

   int projectId;
   try {
     projectId = ((Integer)parameters.firstElement()).intValue();
   } catch (Exception x) {
     throw new XmlRpcException(2, "Invalid ID number: "+parameters.firstElement().toString());
   }

   DropData dropData;
   try {
     DropUseBean dropUseBean = new DropUseBean();
     dropData = dropUseBean.getLastDrop(projectId);
   } catch (Exception x) {
     throw new XmlRpcException(2, "Unable to get drop data: "+x.toString());
   }

   if ( dropData == null )
     throw new XmlRpcException(2, "No drop found");

   return dropData2Hashtable(dropData);
 }


 private Hashtable getUserData(Vector parameters)
        throws XmlRpcException
 {
   if ( parameters.size() < 1 ) {
     throw new XmlRpcException(1, "No username specified");
   }

   String username;
   try {
     username = (String)parameters.firstElement();
   } catch (Exception x) {
     throw new XmlRpcException(2, "Invalid username: "+parameters.firstElement().toString());
   }

   UserData userData;
   try {
     UserUseBean userUseBean = new UserUseBean();
     userData = userUseBean.getUserData(username);
   } catch (Exception x) {
     throw new XmlRpcException(2, "Unable to get user data: "+x.toString());
   }

   if ( userData == null )
     throw new XmlRpcException(2, "User not found");

   return userData2Hashtable(userData);
 }


 private String createUserAlert(Vector parameters)
        throws XmlRpcException
 {
   if ( parameters.size() < 1 ) {
     throw new XmlRpcException(1, "No alert data specified");
   }

   UserAlertData userAlertData = new UserAlertData();
   try {
     Hashtable input = (Hashtable)parameters.firstElement();
     userAlertData.setUsername((String)input.get("username"));
     userAlertData.setSubject((String)input.get("subject"));
     userAlertData.setMessage((String)input.get("message"));
     userAlertData.setProjectId(((Integer)input.get("projectId")).intValue());
     userAlertData.setProjectName((String)input.get("projectName"));
     userAlertData.setSiteName((String)input.get("siteName"));
     userAlertData.setDropVersion(((Integer)input.get("dropVersion")).intValue());
     userAlertData.setActionRequired("true".equalsIgnoreCase((String)input.get("actionRequired")));
     userAlertData.setDate(new java.util.Date());
   } catch (Exception x) {
     throw new XmlRpcException(2, "Invalid alert data: "+x.toString());
   }

   try {
     UserAlertUseBean userAlertUseBean = new UserAlertUseBean();
     userAlertUseBean.createUserAlert(userAlertData);
   } catch (Exception x) {
     throw new XmlRpcException(2, "Unable to create userAlert: "+x.toString());
   }
   
   return "SUCCESS";
 }


 private Hashtable userData2Hashtable(UserData userData)
 {
   Hashtable hash = new Hashtable();

   hash.put("username", userData.getUsername());
   // hash.put("password", userData.getPassword());
   hash.put("mainContactName", userData.getMainContactName()!=null ? userData.getMainContactName() : "");
   hash.put("mainContactEmail", userData.getMainContactEmail()!=null ? userData.getMainContactEmail() : "");
   hash.put("secondaryContactName", userData.getSecondaryContactName()!=null ? userData.getSecondaryContactName() : "");
   hash.put("secondaryContactEmail", userData.getSecondaryContactEmail()!=null ? userData.getSecondaryContactEmail() : "");
   hash.put("role", userData.getRole());
   hash.put("phone", userData.getPhone());
   hash.put("fax", userData.getFax());
   hash.put("mobilePhoneNumber", userData.getMobilePhoneNumber()!=null ? userData.getMobilePhoneNumber() : "");
   hash.put("postalAddress", userData.getPostalAddress()!=null ? userData.getPostalAddress() : "");
   hash.put("company", userData.getCompany()!=null ? userData.getCompany() : "");
   hash.put("country", userData.getCountry());
   // hash.put("regions", new Vector(userData.getRegions()) );
   // hash.put("usergroups", new Vector(userData.getUsergroups()) );
   // hash.put("unreadMessageNotificationFrequency", Frequency.fromInt(userData.getUnreadMessageNotificationFrequency()).getLabel());
   // if ( userData.getLastLoginDate() != null ) {
   //   hash.put("lastLoginDate", userData.getLastLoginDate());
   // }
   // if ( userData.getLastEmailDate() != null ) {
   //   hash.put("lastEmailDate", userData.getLastEmailDate());
   // }

   return hash;
 }

 private Hashtable dropData2Hashtable(DropData dropData)
 {
   Hashtable hash = new Hashtable();

   hash.put("projectId", new Integer(dropData.getProjectId()));
   hash.put("version", new Integer(dropData.getVersion()));
   hash.put("projectName", dropData.getProjectName()!=null ? dropData.getProjectName() : "");
   hash.put("dropName", dropData.getDropName()!=null ? dropData.getDropName() : "");
   hash.put("description", dropData.getDescription()!=null ? dropData.getDescription() : "");
   hash.put("status", DropStatus.fromInt(dropData.getStatus()).getLabel());
   hash.put("archived", dropData.getArchived() ? "true" : "false");
   // hash.put("comments", dropData.getComments()!=null ? dropData.getComments() : "");
   hash.put("deploymentType", dropData.getDeploymentType()==0 ? "Automatic" : "Manual" );
   if ( dropData.getDroppedDate() != null ) {
     hash.put("droppedDate", dropData.getDroppedDate());
     hash.put("droppedBy", dropData.getDroppedBy()!=null ? dropData.getDroppedBy() : "");
   }
   if ( dropData.getStagingDate() != null ) {
     hash.put("stagingDate", dropData.getStagingDate());
     hash.put("stagingBy", dropData.getStagingBy()!=null ? dropData.getStagingBy() : "");
   }
   if ( dropData.getQaDate() != null ) {
     hash.put("qaDate", dropData.getQaDate());
     hash.put("qaBy", dropData.getQaBy()!=null ? dropData.getQaBy() : "");
   }
   if ( dropData.getProducerDate() != null ) {
     hash.put("producerDate", dropData.getProducerDate());
     hash.put("producerBy", dropData.getProducerBy()!=null ? dropData.getProducerBy() : "");
   }
   if ( dropData.getLiveDeployDate() != null ) {
     hash.put("liveDeployDate", dropData.getLiveDeployDate());
     hash.put("liveDeployBy", dropData.getLiveDeployBy()!=null ? dropData.getLiveDeployBy() : "");
   }
   if ( dropData.getLiveRemoveDate() != null ) {
     hash.put("liveRemoveDate", dropData.getLiveRemoveDate());
     hash.put("liveRemoveBy", dropData.getLiveRemoveBy()!=null ? dropData.getLiveRemoveBy() : "");
   }
   // Vector files = new Vector();
   // for (Iterator i = dropData.getFiles().iterator(); i.hasNext() ;) {
   //   files.add(((FileData)i.next()).getPathname());
   // }
   // hash.put("files", files);

   return hash;
 }

 private Hashtable projectData2Hashtable(ProjectData projectData)
 {
   Hashtable hash = new Hashtable();

   hash.put("projectId", new Integer(projectData.getProjectId()));
   hash.put("name", projectData.getName());
   hash.put("description", projectData.getDescription()!=null ? projectData.getDescription() : "");
   hash.put("siteName", projectData.getSiteName());
   hash.put("path", projectData.getPath());
   hash.put("developmentHostname", projectData.getDevelopmentHostname());
   hash.put("stagingHostname", projectData.getStagingHostname());
   hash.put("liveHostname", projectData.getLiveHostname());
   hash.put("producer", projectData.getProducer());
   hash.put("developer", projectData.getDeveloper());
   hash.put("type", projectData.getType());
   hash.put("continuous", projectData.isContinuous() ? "true" : "false" );
   hash.put("businessUnitName", projectData.getBusinessUnitName());
   hash.put("businessUnitContactDetails", projectData.getBusinessUnitContactDetails());
   // hash.put("comments", projectData.getProjectComments()!=null ? projectData.getProjectComments() : "");
   hash.put("contentEditor", projectData.getContentEditor()!=null ? projectData.getContentEditor() : "");
   if ( projectData.getScheduledUpdateFrequency() != null ) {
     hash.put("scheduledUpdateFrequency", projectData.getScheduledUpdateFrequency());
   }
   if ( projectData.getArchivingPeriod() != null ) {
     hash.put("archivingPeriod", projectData.getArchivingPeriod());
   }
   if ( projectData.getDateCreated() != null ) {
     hash.put("dateCreated", projectData.getDateCreated());
   }
   if ( projectData.getDateApproved() != null ) {
     hash.put("dateApproved", projectData.getDateApproved());
   }
   if ( projectData.getDateModified() != null ) {
     hash.put("dateModified", projectData.getDateModified());
   }
   if ( projectData.getDateAssetsDelivered() != null ) {
     hash.put("dateAssetsDelivered", projectData.getDateAssetsDelivered());
   }
   if ( projectData.getDateLiveTarget() != null ) {
     hash.put("dateLiveTarget", projectData.getDateLiveTarget());
   }
   if ( projectData.getDateScheduledRemoval() != null ) {
     hash.put("dateScheduledRemoval", projectData.getDateScheduledRemoval());
   }
   if ( projectData.getDateRemovedFromLive() != null ) {
     hash.put("dateRemovedFromLive", projectData.getDateRemovedFromLive());
   }

   return hash;
 }

}
