/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CaptiveCollector plugin for Solaris.
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

package com.capmet.plugins.CaptiveCollector.Solaris;

import java.util.TreeMap;
import com.capmet.metrics.rmi.PerformanceException;
import com.capmet.metrics.rmi.CategorySource;
import com.capmet.metrics.rmi.CategoryClone;
import com.capmet.metrics.rmi.FieldSource;
import com.capmet.metrics.jmdr.mdr_exception_t;

public class PerCPUData implements CategorySource
{
    private mdr_sun_cpu_t cpu;

    public PerCPUData(
        TreeMap<String, String> localToEnUsMap,
        TreeMap<String, String> enUsToLocalMap)
            throws PerformanceException
    {
        try
        {
            cpu = new mdr_sun_cpu_t(localToEnUsMap, enUsToLocalMap);
        }
        catch(Exception e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String getCategoryName()
    {
        return SolarisMetrics.PER_CPU_DATA;
    }

    public boolean isCategorySupported()
    {
        return true;
    }

    public boolean isSingleton()
    {
        return false;
    }

    public int getFieldCount()
    {
        return mdr_sun_cpu_t.MSC_FIELD_COUNT;
    }

    public boolean isFieldSupported(int fieldNumber)
        throws PerformanceException
    {
        return true;
    }

    public String getFieldName(int fieldNumber)
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_field_name(fieldNumber);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String getFieldExplanation(String fieldName)
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_field_explanation(
                cpu.mdr_field_name_to_number(fieldName));
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String getFieldExplanation(int fieldNumber)
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_field_explanation(fieldNumber);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String getCategoryExplanation()
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_category_explanation();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public int getFieldNumber(String fieldName)
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_field_name_to_number(fieldName);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public FieldSource getField(int fieldNumber)
        throws PerformanceException
    {
        return new MDRValue(cpu, fieldNumber);
    }

    public FieldSource getField(String fieldName)
        throws PerformanceException
    {
        try
        {
            return new MDRValue(cpu, cpu.mdr_field_name_to_number(fieldName));
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public FieldSource getField(int fieldNumber, String instanceName)
        throws PerformanceException
    {
        return new MDRValue(cpu, fieldNumber, instanceName);
    }

    public FieldSource getField(String fieldName, String instanceName)
        throws PerformanceException
    {
        try
        {
            return new MDRValue(cpu,
                cpu.mdr_field_name_to_number(fieldName), instanceName);
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String[] getInstanceNames()
        throws PerformanceException
    {
        return cpu.mdr_keyset();
    }

    public String[] getAllFieldNames()
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_all_field_names();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public String[] getSupportedFieldNames()
        throws PerformanceException
    {
        try
        {
            return cpu.mdr_supported_field_names();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public void categoryRefresh()
        throws PerformanceException
    {
        try
        {
            cpu.mdr_refresh();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }

    public void categoryRefreshRates()
        throws PerformanceException
    {
        try
        {
            cpu.mdr_refresh_rates();
        }
        catch(mdr_exception_t e)
        {
            throw new PerformanceException(e.getMessage());
        }
    }
}
