#ifndef UTILS_H
#define UTILS_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <simpleble/Peripheral.h>

using namespace godot;

class BLEUtils {
	// Public methods
public:
	/**
	 * Adpaters and peripherals status
	 */
	enum Status {
		NOT_INITIALIZED,
		NOT_CONNECTED,
		INVALID_REFERENCE,
		SERVICE_NOT_FOUND,
		CHARACTERISTIC_NOT_FOUND,
		DESCRIPTOR_NOT_FOUND,
		OPERATION_NOT_SUPPORTED,
		OPERATION_FAILED,
		INTERNAL_ERROR,
		NOT_FOUND,
		ALREADY_CONNECTED,
		ALREADY_DISCONNECTED,
		UNKNOWN
	};

	/**
	 * Adpaters and peripherals status level
	 */
	enum StatusLevel {
		INFO,
		WARNING,
		ERROR
	};

	/**
	 * Constructor
	 */
	BLEUtils() = default;

	/**
	 * Get manufacturer data as dictionary
	 * @param p_manufacturer_data manufacturer data
	 * @return manufacturer data as dictionary
	 */
	static const Dictionary get_manufacturer_data(const std::map<uint16_t, SimpleBLE::ByteArray> &p_manufacturer_data);

	/**
	 * Get all services as dictionary
	 * @param p_services services
	 * @return services as dictionary
	 */
	static const Dictionary get_services(std::vector<SimpleBLE::Service> &p_services);

	/**
	 * Transform ByteArray to PackedByteArray
	 * @param p_bytes SimpleBLE ByteArray
	 * @return Godot PackedByteArray
	 */
	static const PackedByteArray string_to_byte_array(const SimpleBLE::ByteArray &p_bytes);

	/**
	 * Get status from SimpleBLE exception
	 * @param p_exception exception
	 * @return status
	 */
	static const Status get_status(std::exception &p_exception);

	/**
	 * Get string form the status
	 * @param p_code status
	 * @return string status
	 */
	static String get_status_string(const Status &p_code);

	/**
	 * Get string form the status level
	 * @param p_level status level
	 * @return string status level
	 */
	static String get_status_level_string(const StatusLevel &p_level);
};

#endif // UTILS_H
