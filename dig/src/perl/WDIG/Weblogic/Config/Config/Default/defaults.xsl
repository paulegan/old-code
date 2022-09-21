<?xml version="1.0" encoding="ISO-8859-1"?>

<!--

  $Id$

  StyleSheet to add default attributes to an XML doc from an XML Schema

  paul.x.egan.-nd@email.disney.com 20030423

  -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

 <xsl:param name="schema_file" select="'defaults.xsd'"/>
 <xsl:variable name="all_defaults" select="document($schema_file)/schema/element//attribute[ @use='default' and @value ]"/>

 <xsl:template match="*">

  <xsl:element name="{name()}">

   <xsl:variable name="present_attribs" select="./@*"/>
   <xsl:for-each select="$present_attribs">
    <xsl:attribute name="{name()}"><xsl:value-of select="."/></xsl:attribute>
   </xsl:for-each>

   <xsl:variable name="default_attribs" select="$all_defaults[ ancestor::element[ @name=name(current()) ] ]"/>
   <xsl:for-each select="$default_attribs">
    <xsl:if test="not($present_attribs[ name(.)=current()/@name ])">
     <xsl:attribute name="{@name}"><xsl:value-of select="@value"/></xsl:attribute>
    </xsl:if>
   </xsl:for-each>

   <xsl:apply-templates/>

  </xsl:element>

 </xsl:template>

</xsl:stylesheet>
