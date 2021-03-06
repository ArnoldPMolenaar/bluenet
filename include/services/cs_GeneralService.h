/**
 * Author: Dominik Egger
 * Copyright: Distributed Organisms B.V. (DoBots)
 * Date: Oct 22, 2014
 * License: LGPLv3+, Apache License, or MIT, your choice
 */
#pragma once

#include <vector>

#include "characteristics/cs_MeshMessage.h"
#include "characteristics/cs_UuidConfig.h"
#include "common/cs_Storage.h"
#include "cs_BluetoothLE.h"
#include "processing/cs_Temperature.h"

#define GENERAL_UUID "f5f90000-59f9-11e4-aa15-123b93f75cba"

/* General Service for the Crownstone
 *
 * There are several characteristics that fit into the general service description. There is a characteristic
 * that measures the temperature, there are several characteristics that defines the crownstone, namely by
 * name, by type, or by location (room), and there is a characteristic to update its firmware.
 *
 * If meshing is enabled, it is also possible to send a message into the mesh network using a characteristic.
 */
class GeneralService: public BLEpp::GenericService {
public:
	GeneralService(BLEpp::Nrf51822BluetoothStack &stack);

	/* Update the temperature characteristic.
	*/
	void setTemperature(int32_t temperature);

	/* Perform non urgent functionality every main loop.
	 *
	 * Every component has a "tick" function which is for non-urgent things. Urgent matters have to be 
	 * resolved immediately in interrupt service handlers. The temperature for example is updated every 
	 * tick, because timing is not important for this at all.
	 */
	void tick();

	/** Helper function to generate a GeneralService object
	*/
	static GeneralService& createService(BLEpp::Nrf51822BluetoothStack& stack);

protected:
	BLEpp::Nrf51822BluetoothStack* _stack;

	// References to characteristics
	BLEpp::CharacteristicT<int32_t>* _temperatureCharacteristic;
	BLEpp::Characteristic<std::string>* _nameCharacteristic;
	BLEpp::Characteristic<std::string>* _deviceTypeCharacteristic;
	BLEpp::Characteristic<std::string>* _roomCharacteristic;
	BLEpp::Characteristic<int32_t>* _firmwareCharacteristic;
	BLEpp::Characteristic<MeshMessage>* _meshCharacteristic;

	/* Enable the temperature characteristic.
	*/
	void addTemperatureCharacteristic();
	/* Enable the change name characteristic.
	*/
	void addChangeNameCharacteristic();
	/* Enable the device type characteristic.
	*/
	void addDeviceTypeCharacteristic();
	/* Enable the room characteristic.
	 *
	 * The room needs to be set by the user. There is not yet functionality in place in the crownstone 
	 * software to figure this out for itself.
	 */
	void addRoomCharacteristic();
	/* Enable the firmware upgrade characteristic.
	*/
	void addFirmwareCharacteristic();
	/* Enable the mesh characteristic.
	*/
	void addMeshCharacteristic();

	/* Retrieve the Bluetooth name from the object representing the BLE stack.
	*/
	std::string&  getBLEName();
	/* Write the Bluetooth name to the object representing the BLE stack.
	 *
	 * This updates the Bluetooth name immediately, however, it does not update the name persistently. It 
	 * has to be written to FLASH in that case.
	 */
	void setBLEName(const std::string &name);

	/* Get a handle to the persistent storage struct and load it from FLASH.
	 *
	 * Persistent storage is implemented in FLASH. Just as with SSDs, it is important to realize that 
	 * writing less than a minimal block strains the memory just as much as flashing the entire block. 
	 * Hence, there is an entire struct that can be filled and flashed at once.
	 */
	void loadPersistentStorage();

	/* Save to FLASH.
	*/
	void savePersistentStorage();
private:
	std::string _name;
	std::string _room;
	std::string _type;

	pstorage_handle_t _storageHandle;
	ps_general_service_t _storageStruct;
};
