/*
 * generated by appdef from Audemo.ad
 */

/*  $NCDId: @(#)resources.h,v 1.4 1993/12/03 02:30:15 greg Exp $ */
"*font:                           *courier-medium-r-normal*140*",
"*Label.borderWidth:              0",
"*resizable:                      true",
"*top:                            chainTop",
"*bottom:                         chainTop",
"*left:                           chainLeft",
"*right:                          chainLeft",
"*Text*right:                     chainRight",
"*Text.translations:              #override\\n\
	<Key>Return: beginning-of-line()",
"*mainWin.play.label:             Play",
"*mainWin.record.label:           Record",
"*mainWin.record.fromHoriz:       play",
"*mainWin.buckets.label:          Buckets",
"*mainWin.buckets.fromHoriz:      record",
"*mainWin.meterToggle.label:      Meter",
"*mainWin.meterToggle.fromHoriz:  buckets",
"*mainWin.rescan.label:           Rescan",
"*mainWin.rescan.fromHoriz:       meterToggle",
"*mainWin.quit.label:             Quit",
"*mainWin.quit.fromHoriz:         rescan",
"*mainWin.version.fromHoriz:      quit",
"*mainWin.leftMeterLabel.label:   \\ Left:",
"*mainWin.leftMeterLabel.fromVert: play",
"*mainWin.leftMeter.fromHoriz:    leftMeterLabel",
"*mainWin.leftMeter.fromVert:     play",
"*mainWin.leftMeter.orientation:  horizontal",
"*mainWin.leftMeter.sensitive:    false",
"*mainWin.leftMeter.minimumThumb: 0",
"*mainWin.leftMeter.right:        chainRight",
"*mainWin.rightMeterLabel.label:  Right:",
"*mainWin.rightMeterLabel.fromVert: leftMeterLabel",
"*mainWin.rightMeter.fromHoriz:   rightMeterLabel",
"*mainWin.rightMeter.fromVert:    leftMeterLabel",
"*mainWin.rightMeter.orientation: horizontal",
"*mainWin.rightMeter.sensitive:   false",
"*mainWin.rightMeter.minimumThumb: 0",
"*mainWin.rightMeter.right:       chainRight",
"*mainWin.volumeLabel.fromVert:   rightMeterLabel",
"*mainWin.volume.fromHoriz:       volumeLabel",
"*mainWin.volume.fromVert:        rightMeterLabel",
"*mainWin.volume.orientation:     horizontal",
"*mainWin.volume.right:           chainRight",
"*mainWin.info.fromVert:          volumeLabel",
"*mainWin.info.scrollVertical:    whenNeeded",
"*mainWin.info.height:            150",
"*mainWin.info.wrap:              word",
"*mainWin.info.displayCaret:      false",
"*mainWin.info*string:            \
Select a file from the list below.  Single clicking on the file will \
display information about the file in this window.  Double clicking on \
the file or clicking on the 'Play' button will play the file.",
"*mainWin.directoryLabel.label:   Directory:",
"*mainWin.directoryLabel.fromVert: info",
"*mainWin.directory.fromVert:     info",
"*mainWin.directory.fromHoriz:    directoryLabel",
"*mainWin.directory*editType:     edit",
"*mainWin.templateLabel.label:    \\ Template:",
"*mainWin.templateLabel.fromVert: directoryLabel",
"*mainWin.template.fromVert:      directoryLabel",
"*mainWin.template.fromHoriz:     templateLabel",
"*mainWin.template*string:        *.snd *.au *.wav *.voc",
"*mainWin.template*editType:      edit",
"*mainWin.viewport.fromVert:      templateLabel",
"*mainWin.viewport.right:         chainRight",
"*mainWin.viewport.height:        150",
"*mainWin.viewport.allowVert:     true",
"*bucketPop.title:                Buckets",
"*bucketWin.query.label:          Query",
"*bucketWin.play.label:           Play",
"*bucketWin.play.fromHoriz:       query",
"*bucketWin.record.label:         Record",
"*bucketWin.record.fromHoriz:     play",
"*bucketWin.load.label:           Load",
"*bucketWin.load.fromHoriz:       record",
"*bucketWin.save.label:           Save",
"*bucketWin.save.fromHoriz:       load",
"*bucketWin.delete.label:         Delete",
"*bucketWin.delete.fromHoriz:     save",
"*bucketWin.dismiss.label:        Dismiss",
"*bucketWin.dismiss.fromHoriz:    delete",
"*bucketWin.format.label:         Format Key",
"*bucketWin.format.fromHoriz:     dismiss",
"*bucketWin.format.menuName:      formatMenu",
"*bucketWin.format.horizDistance: 20",
"*bucketWin.access.label:         Access Key",
"*bucketWin.access.fromHoriz:     format",
"*bucketWin.access.menuName:      accessMenu",
"*bucketWin.header.fromVert:      query",
"*bucketWin.header.label:         \
\\    Description       Duration  Tracks  Frequency  Format  Access",
"*bucketWin.viewport.fromVert:    header",
"*bucketWin.viewport.forceBars:   true",
"*bucketWin.viewport.allowVert:   true",
"*bucketWin.viewport.height:      100",
"*savePop.title:                  Save bucket",
"*saveDialog.label:               Filename:",
"*saveDialog.value:               ",
"*recordPop.title:                Record",
"*recordWin.Text*editType:        edit",
"*recordWin.filenameLabel.label:  \\    Filename:",
"*recordWin.filename.fromHoriz:   filenameLabel",
"*recordWin.filename*string:      record",
"*recordWin.fileFormatLabel.label: \\ File Format:",
"*recordWin.fileFormatLabel.fromVert: filenameLabel",
"*recordWin.fileFormatMenuButton.fromHoriz: fileFormatLabel",
"*recordWin.fileFormatMenuButton.fromVert: filenameLabel",
"*recordWin.fileFormatMenuButton.menuName: fileFormatMenu",
"*recordWin.fileFormatMenuButton.right: chainRight",
"*recordWin.dataFormatLabel.label: \\ Data Format:",
"*recordWin.dataFormatLabel.fromVert: fileFormatLabel",
"*recordWin.dataFormatMenuButton.fromHoriz: dataFormatLabel",
"*recordWin.dataFormatMenuButton.fromVert: fileFormatLabel",
"*recordWin.dataFormatMenuButton.menuName: dataFormatMenu",
"*recordWin.dataFormatMenuButton.right: chainRight",
"*recordWin.durationLabel.label:  Max Duration:",
"*recordWin.durationLabel.fromVert: dataFormatLabel",
"*recordWin.duration.fromHoriz:   durationLabel",
"*recordWin.duration.fromVert:    dataFormatLabel",
"*recordWin.duration*string:      10",
"*recordWin.duration.right:       chainLeft",
"*recordWin.readOnly.label:       Read Only",
"*recordWin.readOnly.fromHoriz:   duration",
"*recordWin.readOnly.fromVert:    dataFormatLabel",
"*recordWin.frequencyLabel.label: \\   Frequency:",
"*recordWin.frequencyLabel.fromVert: readOnly",
"*recordWin.frequency.fromHoriz:  frequencyLabel",
"*recordWin.frequency.fromVert:   readOnly",
"*recordWin.frequency*string:     8000",
"*recordWin.frequency.right:      chainLeft",
"*recordWin.lineMode.label:       Mic",
"*recordWin.lineMode.fromHoriz:   frequency",
"*recordWin.lineMode.fromVert:    readOnly",
"*recordWin.commentLabel.label:   Comment:",
"*recordWin.commentLabel.fromVert: frequency",
"*recordWin.comment.fromVert:     commentLabel",
"*recordWin.gainLabel.label:      Gain: 100%",
"*recordWin.gainLabel.fromVert:   comment",
"*recordWin.gain.fromHoriz:       gainLabel",
"*recordWin.gain.fromVert:        comment",
"*recordWin.gain.orientation:     horizontal",
"*recordWin.gain.right:           chainRight",
"*recordWin.Command.fromVert:     gainLabel",
"*recordWin.record.label:         Record",
"*recordWin.monitor.label:        Monitor",
"*recordWin.monitor.fromHoriz:    record",
"*recordWin.new.label:            New Bucket",
"*recordWin.new.fromHoriz:        monitor",
"*recordWin.dismiss.label:        Dismiss",
"*recordWin.dismiss.fromHoriz:    new",
"*savePop.geometry:               300",
"*saveWin.filenameLabel.label:    \\   Filename:",
"*saveWin.filename.fromHoriz:     filenameLabel",
"*saveWin.filename*editType:      edit",
"*saveWin.fileFormatLabel.label:  File format:",
"*saveWin.fileFormatLabel.fromVert: filenameLabel",
"*saveWin.fileFormatMenuButton.fromVert: filenameLabel",
"*saveWin.fileFormatMenuButton.fromHoriz: fileFormatLabel",
"*saveWin.fileFormatMenuButton.menuName: fileFormatMenu",
"*saveWin.fileFormatMenuButton.right: chainRight",
"*saveWin.ok.label:               Ok",
"*saveWin.ok.fromVert:            fileFormatLabel",
"*saveWin.cancel.label:           Cancel",
"*saveWin.cancel.fromHoriz:       ok",
"*saveWin.cancel.fromVert:        fileFormatLabel",
