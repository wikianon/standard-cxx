#include <stdio.h>
#include <Pdh.h>
#include <PdhMsg.h>
#include "mdr_exception.h"

namespace mdr {

const char *mdr_exception_t::PdhErrorMessage(PDH_STATUS status)
{
	static char unknown[128];

	switch(status)
	{
	case PDH_CSTATUS_VALID_DATA:
		return "The returned data is valid.";
	case PDH_CSTATUS_NEW_DATA:
		return "The return data value is valid and different from the last sample.";
	case PDH_CSTATUS_NO_MACHINE:
		return "Unable to connect to specified machine or machine is off line.";
	case PDH_CSTATUS_NO_INSTANCE:
		return "The specified instance is not present.";
	case PDH_MORE_DATA:
		return "There is more data to return than would fit in the supplied buffer. Allocate a larger buffer and call the function again.";
	case PDH_CSTATUS_ITEM_NOT_VALIDATED:
		return "The data item has been added to the query, but has not been validated nor accessed. No other status information on this data item is available.";
	case PDH_RETRY:
		return "The selected operation should be retried.";
	case PDH_NO_DATA:
		return "No data to return.";
	case PDH_CALC_NEGATIVE_DENOMINATOR:
		return "A counter with a negative denominator value was detected.";
	case PDH_CALC_NEGATIVE_TIMEBASE:
		return "A counter with a negative timebase value was detected.";
	case PDH_CALC_NEGATIVE_VALUE:
		return "A counter with a negative value was detected.";
	case PDH_DIALOG_CANCELLED:
		return "The user cancelled the dialog box.";
	case PDH_END_OF_LOG_FILE:
		return "The end of the log file was reached.";
	case PDH_ASYNC_QUERY_TIMEOUT:
		return "Time out while waiting for asynchronous counter collection thread to end.";
	case PDH_CANNOT_SET_DEFAULT_REALTIME_DATASOURCE:
		return "Cannot change default real-time data source. There are real-time query sessions collecting counter data.";
	case PDH_CSTATUS_NO_OBJECT:
		return "The specified object is not found on the system.";
	case PDH_CSTATUS_NO_COUNTER:
		return "The specified counter could not be found.";
	case PDH_CSTATUS_INVALID_DATA:
		return "The returned data is not valid.";
	case PDH_MEMORY_ALLOCATION_FAILURE:
		return "A PDH function could not allocate enough temporary memory to complete the operation. Close some applications or extend the pagefile and retry the function.";
	case PDH_INVALID_HANDLE:
		return "The handle is not a valid PDH object.";
	case PDH_INVALID_ARGUMENT:
		return "A required argument is missing or incorrect.";
	case PDH_FUNCTION_NOT_FOUND:
		return "Unable to find the specified function.";
	case PDH_CSTATUS_NO_COUNTERNAME:
		return "No counter was specified.";
	case PDH_CSTATUS_BAD_COUNTERNAME:
		return "Unable to parse the counter path. Check the format and syntax of the specified path.";
	case PDH_INVALID_BUFFER:
		return "The buffer passed by the caller is invalid.";
	case PDH_INSUFFICIENT_BUFFER:
		return "The requested data is larger than the buffer supplied. Unable to return the requested data.";
	case PDH_CANNOT_CONNECT_MACHINE:
		return "Unable to connect to the requested machine.";
	case PDH_INVALID_PATH:
		return "The specified counter path could not be interpreted.";
	case PDH_INVALID_INSTANCE:
		return "The instance name could not be read from the specified counter path.";
	case PDH_INVALID_DATA:
		return "The data is not valid.";
	case PDH_NO_DIALOG_DATA:
		return "The dialog box data block was missing or invalid.";
	case PDH_CANNOT_READ_NAME_STRINGS:
		return "Unable to read the counter and/or explain text from the specified machine.";
	case PDH_LOG_FILE_CREATE_ERROR:
		return "Unable to create the specified log file.";
	case PDH_LOG_FILE_OPEN_ERROR:
		return "Unable to open the specified log file.";
	case PDH_LOG_TYPE_NOT_FOUND:
		return "The specified log file type has not been installed on this system.";
	case PDH_NO_MORE_DATA:
		return "No more data is available.";
	case PDH_ENTRY_NOT_IN_LOG_FILE:
		return "The specified record was not found in the log file.";
	case PDH_DATA_SOURCE_IS_LOG_FILE:
		return "The specified data source is a log file.";
	case PDH_DATA_SOURCE_IS_REAL_TIME:
		return "The specified data source is the current activity.";
	case PDH_UNABLE_READ_LOG_HEADER:
		return "The log file header could not be read.";
	case PDH_FILE_NOT_FOUND:
		return "Unable to find the specified file.";
	case PDH_FILE_ALREADY_EXISTS:
		return "There is already a file with the specified file name.";
	case PDH_NOT_IMPLEMENTED:
		return "The function referenced has not been implemented.";
	case PDH_STRING_NOT_FOUND:
		return "Unable to find the specified string in the list of performance name and explain text strings.";
	case PDH_UNABLE_MAP_NAME_FILES:
		return "Unable to map to the performance counter name data files. The data will be read from the registry and stored locally.";
	case PDH_UNKNOWN_LOG_FORMAT:
		return "The format of the specified log file is not recognized by the PDH DLL.";
	case PDH_UNKNOWN_LOGSVC_COMMAND:
		return "The specified Log Service command value is not recognized.";
	case PDH_LOGSVC_QUERY_NOT_FOUND:
		return "The specified Query from the Log Service could not be found or could not be opened.";
	case PDH_LOGSVC_NOT_OPENED:
		return "The Performance Data Log Service key could not be opened. This may be due to insufficient privilege or because the service has not been installed.";
	case PDH_WBEM_ERROR:
		return "An error occurred while accessing the WBEM data store.";
	case PDH_ACCESS_DENIED:
		return "Unable to access the desired machine or service. Check the permissions and authentication of the log service or the interactive user session against those on the machine or service being monitored.";
	case PDH_LOG_FILE_TOO_SMALL:
		return "The maximum log file size specified is too small to log the selected counters. No data will be recorded in this log file. Specify a smaller set of counters to log or a larger file size and retry this call.";
	case PDH_INVALID_DATASOURCE:
		return "Cannot connect to ODBC DataSource Name.";
	case PDH_INVALID_SQLDB:
		return "SQL Database does not contain a valid set of tables for Perfmon, use PdhCreateSQLTables.";
	case PDH_NO_COUNTERS:
		return "No counters were found for this Perfmon SQL Log Set.";
	case PDH_SQL_ALLOC_FAILED:
		return "Call to SQLAllocStmt failed with %1.";
	case PDH_SQL_ALLOCCON_FAILED:
		return "Call to SQLAllocConnect failed with %1.";
	case PDH_SQL_EXEC_DIRECT_FAILED:
		return "Call to SQLExecDirect failed with %1.";
	case PDH_SQL_FETCH_FAILED:
		return "Call to SQLFetch failed with %1.";
	case PDH_SQL_ROWCOUNT_FAILED:
		return "Call to SQLRowCount failed with %1.";
	case PDH_SQL_MORE_RESULTS_FAILED:
		return "Call to SQLMoreResults failed with %1.";
	case PDH_SQL_CONNECT_FAILED:
		return "Call to SQLConnect failed with %1.";
	case PDH_SQL_BIND_FAILED:
		return "Call to SQLBindCol failed with %1.";
	case PDH_CANNOT_CONNECT_WMI_SERVER:
		return "Unable to connect to the WMI server on requested machine.";
	case PDH_PLA_COLLECTION_ALREADY_RUNNING:
		return "Collection is already running.";
	case PDH_PLA_ERROR_SCHEDULE_OVERLAP:
		return "The specified start time is after the end time.";
	case PDH_PLA_COLLECTION_NOT_FOUND:
		return "Collection does not exist.";
	case PDH_PLA_ERROR_SCHEDULE_ELAPSED:
		return "The specified end time has already elapsed.";
	case PDH_PLA_ERROR_NOSTART:
		return "Collection did not start, check the application event log for any errors.";
	case PDH_PLA_ERROR_ALREADY_EXISTS:
		return "Collection already exists.";
	case PDH_PLA_ERROR_TYPE_MISMATCH:
		return "There is a mismatch in the settings type.";
	case PDH_PLA_ERROR_FILEPATH:
		return "The information specified does not resolve to a valid path name.";
	case PDH_PLA_SERVICE_ERROR:
		return "The \"Performance Logs & Alerts\" service did not respond.";
	case PDH_PLA_VALIDATION_ERROR:
		return "The information passed is not valid.";
	case PDH_PLA_VALIDATION_WARNING:
		return "The information passed is not valid.";
	case PDH_PLA_ERROR_NAME_TOO_LONG:
		return "The name supplied is too long.";
	case PDH_INVALID_SQL_LOG_FORMAT:
		return "SQL log format is incorrect. Correct format is \"SQL:<DSN-name>!<LogSet-Name>\".";
	case PDH_COUNTER_ALREADY_IN_QUERY:
		return "Performance counter in PdhAddCounter() call has already been added in the performance query. This counter is ignored.";
	case PDH_BINARY_LOG_CORRUPT:
		return "Unable to read counter information and data from input binary log files.";
	case PDH_LOG_SAMPLE_TOO_SMALL:
		return "At least one of the input binary log files contain fewer than two data samples.";
	case PDH_OS_LATER_VERSION:
		return "The version of the operating system on the computer named %1 is later than that on the local computer. This operation is not available from the local computer.";
	case PDH_OS_EARLIER_VERSION:
		return "%1 supports %2 or later. Check the operating system version on the computer named %3.";
	case PDH_INCORRECT_APPEND_TIME:
		return "The output file must contain earlier data than the file to be appended.";
	case PDH_UNMATCHED_APPEND_COUNTER:
		return "Both files must have identical counters in order to append.";
	case PDH_SQL_ALTER_DETAIL_FAILED:
		return "Cannot alter CounterDetail table layout in SQL database.";
	case PDH_QUERY_PERF_DATA_TIMEOUT:
		return "System is busy. Timeout when collecting counter data. Please retry later or increase \"CollectTime\" registry value.";
	default:
		_snprintf_s(unknown, sizeof unknown, sizeof unknown, "Unknown error: %X", status);
		return unknown;
	}
}

}
