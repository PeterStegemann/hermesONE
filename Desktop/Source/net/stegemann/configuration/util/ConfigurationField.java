package net.stegemann.configuration.util;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention( RetentionPolicy.RUNTIME)
@Target({ ElementType.FIELD, ElementType.TYPE})
public @interface ConfigurationField
{
	String name() default "";
	String itemName() default "";

	boolean ignore() default false;
}
