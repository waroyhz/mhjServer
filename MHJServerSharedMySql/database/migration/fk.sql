
ALTER TABLE `MHJDeviceChild`
  ADD CONSTRAINT `MHJDeviceChild_device_fk`
    FOREIGN KEY (`device`)
    REFERENCES `MHJDeviceBase` (`id`),
  ADD CONSTRAINT `MHJDeviceChild_user_fk`
    FOREIGN KEY (`user`)
    REFERENCES `user_entity` (`id`)
;


ALTER TABLE `MHJDeviceAuthorize`
  ADD CONSTRAINT `MHJDeviceAuthorize_device_fk`
    FOREIGN KEY (`device`)
    REFERENCES `MHJDeviceBase` (`id`),
  ADD CONSTRAINT `MHJDeviceAuthorize_user_fk`
    FOREIGN KEY (`user`)
    REFERENCES `user_entity` (`id`),
  ADD CONSTRAINT `MHJDeviceAuthorize_giveUser_fk`
    FOREIGN KEY (`giveUser`)
    REFERENCES `user_entity` (`id`)
;

ALTER TABLE `MHJDeviceAuthorize`

  ADD CONSTRAINT `MHJDeviceAuthorize_child_fk`
    FOREIGN KEY (`child`)
    REFERENCES `MHJDeviceChild` (`id`)
;


ALTER TABLE `MHJDeviceVirtualDefine`
  ADD CONSTRAINT `MHJDeviceVirtualDefine_deviceKey_fk`
    FOREIGN KEY (`deviceKey`)
    REFERENCES `MHJDeviceChild` (`id`),
  ADD CONSTRAINT `MHJDeviceVirtualDefine_targetKey_fk`
    FOREIGN KEY (`targetKey`)
    REFERENCES `MHJDeviceChild` (`id`)
;
 
