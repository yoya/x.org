/#define/{
	if ($2 == "AMS_MAJOR_VERSION") printf("MS.%s.",$3)
	if ($2 == "AMS_MINOR_VERSION") printf("%s",$3);
}

