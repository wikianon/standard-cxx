/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector application of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package com.capmet.Captivity.CaptiveCollector;

import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import java.net.URLStreamHandlerFactory;
import java.io.InputStream;
import java.io.ByteArrayInputStream;
import java.util.jar.*;
import java.util.zip.ZipFile;
import java.util.zip.ZipEntry;
import java.util.TreeMap;
import java.util.Map;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Locale;
import java.io.File;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import com.capmet.metrics.CmOSFlavor;
import com.capmet.metrics.jmdr.mdr_performance_host_t;
import com.capmet.metrics.jmdr.mdr_exception_t;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.MDRCategory;

// A ClassLoader instance for loading the plugins
class PluginLoader extends ClassLoader
{
    // map a file name to the contents of the file
    private TreeMap<String, String> pmap;

    // convert a url to an input stream of the data in the file
    private class MsgConnection extends URLConnection
    {
        private URL url;

        /**
         * Constructor.
         * @param u The URL of the file.
         */
        public MsgConnection(URL u)
        {
            super(u);
            this.url = u;
        }

        /**
         * Unused.
         */
        public void connect()
        {
        }

        /**
         * Convert the file to an InputStream
         * @return The InputStream object.
         */
        public InputStream getInputStream()
        {
            String f = url.getFile();
            String s = pmap.get(f);

            if (s == null)
                return null;
            
            InputStream is = new ByteArrayInputStream(s.getBytes());

            return is;
        }
    }

    // handle a URL for a given type
    private class MsgStreamHandler extends URLStreamHandler
    {
        /**
         * Open a connect to the URL.
         * @param u The URL
         * @return A new URLConnection object.
         */
        protected URLConnection openConnection(URL u)
        {
            return new MsgConnection(u);
        }
    }

    // Create a new URLStreamHandler for a given protocol.
    private class MsgFactory implements URLStreamHandlerFactory
    {
        /**
         * Create a new URLStreamHandler for a given protocol.
         * @param protocol The protocol in question.
         * @return A new URLStreamHandler object for the protocol.
         */
        public URLStreamHandler createURLStreamHandler(String protocol)
        {
            if (!protocol.equals("msg"))
                return null;

            return new MsgStreamHandler();
        }
    }

    /**
     * The constructor.
     */
    public PluginLoader()
    {
        if (pmap == null)
        {
            URL.setURLStreamHandlerFactory(new MsgFactory());
            pmap = new TreeMap<String, String>();
        }
    }

    /**
     * Define a new class from the raw bytes.
     * @param name The name of the class.
     * @param data The raw data bytes from the jar file.
     * @return A new Class object of the data.
     */
    public Class defineClass(String name, byte[] data)
    {
        Class definedClass = defineClass(name, data, 0, data.length);
        resolveClass(definedClass);
        return definedClass;
    }

    /**
     * Define a new properties file from the raw bytes.
     * @param name The name of the properties file.
     * @param data The raw data bytes from the jar file.
     */
    public void defineProperties(String name, byte[] data)
    {
        pmap.put(name, new String(data));
    }

    /**
     * Look up a resource by name.
     * @param name The name of the resource.
     * @return A new URL object for that resource.
     */
    protected URL findResource(String name)
    {
        try
        {
            return new URL("msg", null, name);
        }
        catch(Exception e)
        {
            return null;
        }
    }
}

/**
 * The source of all stuff for the local computer.
 */
public class LocalHost
{
    // These variables are static because the static block touches them.
    // These need to by synchronized because they are used outside the
    // static block.
    private static mdr_performance_host_t        hostObj;
    private static TreeMap<String, MetricSource> categoryNames;

    // These are used outside the static block but only for read access.
    private static TreeMap<String, String>       localToEnUsMap = null;
    private static TreeMap<String, String>       enUsToLocalMap = null;

    // These are used by the static block only or are not objects.
    private static Vector<MetricSource>          pluginVector;
    private static PluginLoader                  loader;
    private static boolean                       isEnUsLocale;
    private static String                        lastKnownLine = "init";

    // do all that static stuff
    @SuppressWarnings("unchecked")
    private static void doStaticBlock()
    {
        try
        {
            lastKnownLine = "mdr_performance_host_t";

            // allocate the local performance host object
            hostObj = new mdr_performance_host_t();

            // allocate the category names vector
            categoryNames = new TreeMap<String, MetricSource>();

            // no point allocating megabytes for the identity function
            isEnUsLocale = Locale.getDefault().equals(Locale.US);

            // allocate allocate allocate
            if (!isEnUsLocale)
            {
                String localeName = Locale.getDefault().toString();

                CaptiveCollector.log.info(
                    CaptiveCollector.msgs.getString("Locale") + localeName);
                localToEnUsMap = new TreeMap<String, String>();
                enUsToLocalMap = new TreeMap<String, String>();
            }

            // retrieve the plugin directory name from system properties
            String pluginDir = System.getProperty("com.capmet.CaptiveCollector.Top");

            if (pluginDir != null)
            {
                switch(hostObj.mdr_os_flavor)
                {
                case MOS_SOLARIS:
                    pluginDir += "/plugins/CaptiveCollector";
                    pluginDir += "/solaris";
                    break;
                case MOS_LINUX:
                    pluginDir += "/plugins/CaptiveCollector";
                    pluginDir += "/linux";
                    break;
                case MOS_MACOS:
                    pluginDir += "/plugins/CaptiveCollector";
                    pluginDir += "/darwin";
                    break;
                case MOS_BSD:
                    pluginDir += "/plugins/CaptiveCollector";
                    pluginDir += "/freebsd";
                    break;
                case MOS_WINDOWS:
                    pluginDir += "\\plugins\\CaptiveCollector";
                    pluginDir += "\\windows";
                    break;
                }

                lastKnownLine = "new File(" + pluginDir + ")";

                // get a File to it
                File pdir = new File(pluginDir);

                // if it's there
                if (pdir.exists())
                {
                    // get a listing of the files
                    String[] files = pdir.list();

                    // for each of the files
                    for(String f : files)
                    {
                        TreeMap<String, Object> defineLater = new TreeMap<String, Object>();

                        // if it's not a jar file, move on
                        if (!f.endsWith(".jar"))
                            continue;

                        CaptiveCollector.log.info(
                            CaptiveCollector.msgs.getString("Found_Jar") + f);

                        // for all the plugins
                        if (pluginVector == null)
                            pluginVector = new Vector<MetricSource>();

                        // class loader
                        if (loader == null)
                            loader = new PluginLoader();

                        // the class definition of the Metrics interface
                        Class definedMetricClass = null;

                        // make a path name
                        String jarFile = pluginDir + pdir.separatorChar + f;

                        // remove .jar from the end of the file name
                        String classPrefix = f.substring(0, f.length() - 4);

                        // define the names of the classes for the plugin
                        final String metricClassName = classPrefix + "Metrics";

                        // use ZipFile to get the sizes of entries in case getSize() is dumb
                        TreeMap<String, Integer> sizes = new TreeMap<String, Integer>();
                        lastKnownLine = "new ZipFile(" + jarFile + ")";

                        ZipFile zf = new ZipFile(jarFile);

                        Enumeration zum = zf.entries();
                        while (zum.hasMoreElements())
                        {
                            ZipEntry ze = (ZipEntry) zum.nextElement();

                            // make sure we have only slashes, we prefer unix, not windows
                            sizes.put(ze.getName(), new Integer((int) ze.getSize()));
                        }
                        zf.close();

                        lastKnownLine = "new FileInputStream(" + jarFile + ")";

                        // open the jar file
                        FileInputStream fis = new FileInputStream(jarFile);
                        BufferedInputStream bis = new BufferedInputStream(fis);

                        // make it a JarInputStream
                        JarInputStream jis = new JarInputStream(bis, true);

                        // a jar entry
                        JarEntry je;

                        // variables to be collected
                        MetricSource metricClass = null;

                        lastKnownLine = "while(getNextJarEntry())";

                        // while there are more
                        while((je = jis.getNextJarEntry()) != null)
                        {
                            // get the name of the jar entry
                            String name = je.getName();
                            String className = null;

                            // if it's not a class file
                            if (!name.endsWith(".class"))
                            {
                                // if it's not a properties file
                                if (!name.endsWith(".properties"))
                                {
                                    CaptiveCollector.log.warning(
                                        CaptiveCollector.msgs.getString("Unsup_File") + name);

                                    // then move on
                                    continue;
                                }
                            }
                            else
                            {
                                // remove ".class" from the end of the name
                                className = name.substring(0, name.length() - 6);
                            }

                            lastKnownLine = "reading jar file";

                            // read the byte codes from the jar file
                            int bytesLeft = (int) je.getSize();
                            if (bytesLeft == -1)
                                bytesLeft = sizes.get(name).intValue();
                            int offset = 0;
                            byte[] data = new byte[bytesLeft];

                            while(bytesLeft > 0)
                            {
                                int nread = jis.read(data, offset, bytesLeft);
                                if (nread == -1)
                                    break;
                                offset += nread;
                                bytesLeft -= nread;
                            }

                            if (className == null)
                            {
                                lastKnownLine = "loader.defineProperties(" +
                                    name + ", data)";

                                // it's a properties file
                                loader.defineProperties(name, data);
                                continue;
                            }

                            // define this class (jar files use / not separatorChar)
                            className = className.replace('/', '.');
                            Class definedClass = null;

                            lastKnownLine = "loader.defineClass(" + className +
                                ", data)";

                            try {
                                definedClass = loader.defineClass(className, data);
                            } catch(NoClassDefFoundError e) {
                                defineLater.put(className, data);
                            }

                            if (className.contains("."))
                                className = className.substring(
                                    className.lastIndexOf('.') + 1, className.length());

                            if (className.equals(metricClassName))
                                definedMetricClass = definedClass;
                        }
                        jis.close();

                        int tryCount = 0;

                        lastKnownLine = "while(defineLater.size() > 0)";

                        while(defineLater.size() > 0) {
                            if (tryCount++ == 3)
                                throw new RuntimeException("Resolution failure");

                            Vector<String> deleteLater = new Vector<String>();

                            for(Map.Entry<String, Object> me : defineLater.entrySet()) {
                                String name = me.getKey();
                                byte[] data = (byte[]) me.getValue();

                                try {
                                    loader.defineClass(name, data);
                                } catch(NoClassDefFoundError e) {
                                    lastKnownLine = "loader.defineClass(" +
                                        name + ", data): NoClassDefFoundError";
                                    continue;
                                }

                                deleteLater.addElement(name);
                            }

                            lastKnownLine = "defineLater.remove(...)";

                            for(String s : deleteLater)
                                defineLater.remove(s);
                        }

                        if (definedMetricClass == null) {
                            CaptiveCollector.log.warning(
                                CaptiveCollector.msgs.getString("No_Metrics") + f);
                            continue;
                        }

                        CaptiveCollector.log.info(
                            CaptiveCollector.msgs.getString("New_Instance") +
                                metricClassName);

                        lastKnownLine =
                            "metricClass = definedMetricClass.newInstance()";

                        metricClass = ((Class<MetricSource>) definedMetricClass).newInstance();
                        if (!(metricClass instanceof MetricSource)) {
                            CaptiveCollector.log.warning(
                                CaptiveCollector.msgs.getString("Not_MClass") + metricClassName);
                            continue;
                        }

                        lastKnownLine = "metricClass.setNamesMaps(...)";

                        // cause the plugin to map its localized names to en_us names
                        metricClass.setNamesMaps(localToEnUsMap, enUsToLocalMap);

                        pluginVector.addElement(metricClass);

                        CaptiveCollector.log.info(
                            CaptiveCollector.msgs.getString("Jar_Loaded") + f);
                    }
                }
            }

            // load the mdr category names
            for(String s : hostObj.mdr_get_category_names())
                categoryNames.put(s, null);

            // load the plugin category names
            if (pluginVector != null)
                for(MetricSource host : pluginVector)
                    for(String s : host.getCategoryNames())
                        categoryNames.put(s, host);
        }
        catch(Exception e)
        {
            String msg = e.getMessage();

            if (msg == null)
                msg = "null";
            msg = msg + ": " + lastKnownLine;
            CaptiveCollector.log.severe(msg);
            throw new RuntimeException(msg);
        }
    }

    // defer to doStaticBlock()
    static
    {
        doStaticBlock();
    }

    /**
     * Retrieve the flavor of the OS on this host.
     * @return The <CODE>CmOSFlavor</CODE> object.
     */
    public CmOSFlavor getOSFlavor()
    {
        return hostObj.mdr_os_flavor;
    }

    /**
     * Retrieve a <CODE>CategorySource</CODE> object for the
     * specified category.
     * @param categoryName The name of the category in question.
     * @throws PerformanceException Any lower level exception.
     * @return A <CODE>CategorySource</CODE> object.
     */
    public CategorySource getCategory(String categoryName)
        throws PerformanceException
    {
        synchronized(categoryNames) {
            // if it's not in the categoryNames map then go away
            if (!categoryNames.containsKey(categoryName))
                throw new PerformanceException(
                   "LocalHost::getCategory: unknown category: " + categoryName);

            // grab the MetricSource
            MetricSource host = categoryNames.get(categoryName);

            // it's null; it's a native OS category
            if (host == null)
                return new MDRCategory(categoryName);

            // return the plugin category object
            return host.getCategory(categoryName);
        }
    }

    /**
     * Get the names of the categories available on the host.
     * @throws PerformanceException Any lower level exception.
     * @return An array of <CODE>String</CODE> containing the names of
     * the performance categories available.
     */
    public String[] getCategoryNames() throws PerformanceException
    {
        synchronized(categoryNames) {
            int i = 0;
            int size = categoryNames.size();
            String[] names = new String[size];

            for(String s : categoryNames.keySet())
                names[i++] = s;

            return names;
        }
    }

    /**
     * Get the localized version of the en_US string.
     * @param enUsName The en_US string.
     * @throws PerformanceException Any lower level exception.
     * @return The localized version of the string if available.
     */
    public String getLocalizedName(String enUsName)
            throws PerformanceException
    {
        if (isEnUsLocale)
            return enUsName;

        // ask mdr if it knows what the name is
        String lname;

        try
        {
            synchronized(hostObj) {
                lname = hostObj.mdr_localized_name(enUsName);
            }
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }

        // same string returned. obviously not.
        if (lname.equals(enUsName))
        {
            // see if any of the plugins claimed it
            if (enUsToLocalMap.containsKey(enUsName))
                return enUsToLocalMap.get(enUsName);

            // nobody knows. return the id.
            return enUsName;
        }
        else
            return lname;
    }

    /**
     * Get the en_US version of the localized string.
     * @param localizedName The localized string.
     * @throws PerformanceException Any lower level exception.
     * @return The en_US version of the string if available.
     */
    public String getEnUsName(String localizedName)
            throws PerformanceException
    {
        if (isEnUsLocale)
            return localizedName;

        // ask mdr if it knows what the name is
        String us;

        try
        {
            synchronized(hostObj) {
                us = hostObj.mdr_en_us_name(localizedName);
            }
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }

        // same string returned. obviously not.
        if (us.equals(localizedName))
        {
            // see if any of the plugins claimed it
            if (localToEnUsMap.containsKey(localizedName))
                return localToEnUsMap.get(localizedName);

            // nobody knows. return the id.
            return localizedName;
        }
        else
            return us;
    }
}
