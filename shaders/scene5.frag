#define TIME mod(iGlobalTime, 31.0)
#define drum pow(1.0 - 2.0 * mod(TIME, 0.5), 5.0)
vec2 un(vec2 a, vec2 b)
{
  return a.x < b.x ? a : b;
}

float udRoundBox( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}

float sdCappedCylinder( vec3 p, vec2 h )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdCylinder( vec3 p, float r )
{
  return length(p.xz)-r;
}

float specular(vec3 normal, vec3 light, vec3 viewdir, float s)
{
  float nrm = (s + 8.0) / (3.1415 * 8.0);
  float k = max(0.0, dot(viewdir, reflect(light, normal)));
    return  pow(k, s);
}

#define LPOS vec3(0, 0, -2)
vec4 laser(vec3 p) {
  float music = drum; 
  vec3 lightCol = vec3(1.0 * ( 1.0 + sin(TIME))*0.5, 0.5, 0.5);
  float visible = smoothstep(9.2,9.3, TIME);
  float dis = sdCylinder(p.xzy,  0.1 * (1.5 * music + 0.5));
  float distanceToL = max(0.0001, dis);
  vec3 point = (1.5 * music + 0.5) * lightCol * 100.0/(distanceToL*distanceToL);

  return vec4(point * visible, distanceToL);
}

vec4 mirror(vec3 p) {
  float music = drum;
  float s = 15.0;
  vec3 q = mod(p, s) - s * 0.5;
  q.z = p.z;
  float h = clamp( TIME - 2.0, 0.0, 10.0);
  vec3 lightCol = vec3(0.5, 0.5, 1.0);
  float dis = sdCappedCylinder(q.zxy - vec3(-1.0 - h, 0.0, 0.0), vec2(0.01, 2.0));
  float distanceToL = max(0.0001, dis);
  vec3 point = (1.5 * music + 0.5) * lightCol * 40.0/(0.1*distanceToL + 0.3*distanceToL*distanceToL);

  return vec4(point, distanceToL);
}

vec3 evaluateLight(vec3 pos, inout float dis)
{
  vec4 l = laser(pos);
  vec4 m = mirror(pos);
  dis = min(l.w, m.w);
  return l.xyz + m.xyz;
}


void addLightning(inout vec3 color, vec3 normal, vec3 eye, vec3 pos) {
  vec3 lpos = LPOS;

  float dis = length(lpos - pos);
  vec3 invLight = normalize(lpos - pos);
  float diffuse = max(0.0, dot(invLight, normal));
  float spec = specular(normal, -invLight, normalize(eye - pos), 70.0);

  float str = 1.0/(0.1 + 0.01*dis + 0.01*dis*dis);
  float tmp = 0.0;
  color =  color * (0.2 + 0.8*diffuse*evaluateLight(pos, tmp).xyz) + spec*str;
}

float smin( float a, float b)
{
  float k = 2.0;
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float smink( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}
vec2 sun(vec2 a, vec2 b)
{
  float sm = smin(a.x,b.x);
  float ca = abs(sm -a.x);
  float cb = abs(sm -b.x);

  return ca < cb ? vec2(sm, a.y) : vec2(sm, b.y);
}

vec2 sunk(vec2 a, vec2 b, float k)
{
  float sm = smink(a.x,b.x, k);
  float m = min(a.x, b.x);
  float ca = abs(sm -a.x);
  float cb = abs(sm -b.x);
  return ca < cb ? vec2(sm, a.y) : vec2(m, b.y);
}



#define SIZE 8.0
#define PI 3.1415

#define MAT_SPIN 1.0
#define MAT_GROUND 2.0
#define MAT_PILLAR 3.0

vec2 spin(vec3 p) {
  p.x = p.x == 0.0 ? 0.00001 : p.x;
  float angle = atan(p.y, p.x);
  float numParts = 12.0;
  float partSize = PI / numParts;
  angle += PI;
  angle = mod(angle + TIME * 0.2, partSize) + partSize * 0.5 * max(0.0, numParts - 1.0);
  float len = length(p.xy);
  float lenSize = 7.0;
  float lenY = mod(len, lenSize) - lenSize * 0.5;
  float lenPart = floor(len / lenSize);
  float s = 0.5;
  if(lenPart == 0.0){
    s = 0.1 + 0.7 * drum; 
  }
  vec3 newPos = vec3(len * cos(angle), lenY * sin(angle), p.z);
  return vec2(sdCappedCylinder(newPos, vec2(s, 2.5)), MAT_SPIN);
}

vec2 gel(vec3 p) {
  float mt = 10.0;
  float t = TIME;
  float t1 = smoothstep(mt*0.0, mt*1.0, t);
  float t2 = smoothstep(mt*1.0, mt*2.0, t);

  float a = length(p - vec3(0.0, 0.0, -10.0 + t)) - 0.0 - t*t;
  float b = spin(p).x;
  float mdis = a*(1.0-t1) + b*t1*(1.0-t2) + t2*b;
  return vec2(mdis, MAT_SPIN);
}

vec2 ground(vec3 p) {
  float s = 1.0;
  vec3 q = mod(p, s) - s * 0.5;
  q.z = p.z;
  float d = udRoundBox(q, vec3(s * 0.3), s * 0.3);
  return vec2(d, MAT_GROUND);
}

vec2  pillars(vec3 p) {
  float s = 15.0;
  vec3 q = mod(p, s) - s * 0.5;
  q.z = p.z;
  float d = udRoundBox(q, vec3(2.0,0.2, clamp( TIME - 2.0, 0.0, 10.0)), 0.2);
  return vec2(d, MAT_PILLAR);
}

vec2 map(vec3 p) {
  vec2 g = gel(p - vec3(0, 0, 0));
  vec2 solid = sun(pillars(p - vec3(0, 0, 0)), ground(p - vec3(0, 0, 2)));
  vec2 res = sun(g, solid);
  return res;
}

vec3 getNormal(vec3 p)
{
  vec3 normal;
    vec3 ep = vec3(0.01, 0, 0);
    normal.x = map(p + ep.xyz).x - map(p - ep.xyz).x;
    normal.y = map(p + ep.yxz).x - map(p - ep.yxz).x;
    normal.z = map(p + ep.yzx).x - map(p - ep.yzx).x;
    return normalize(normal);

}

float occlusion(vec3 p, vec3 normal)
{
  float o = 2.0*map(p + normal * 0.5).x;
  return 0.2 + 0.8*o;
}

vec3 raymarch(vec3 ro, vec3 rd, inout vec3 finalPos, vec3 eye) {
  float t = 0.0;
  const int maxIter = 100;
  const float maxDis = 200.0;
  float d = 0.0;
  vec3 p = vec3(-1.0, -1.0, -1.0);
  vec3 col = vec3(0);
  const int jumps = 3;
  float ref = 1.0;
  vec3 scatteredLight = vec3(0.0);
  float transmittance = 1.0;
  for (int j = 0; j < jumps; j++) {
    for (int i = 0; i < maxIter; i++) {
      p = ro + rd * t;

        vec2 res = map(p);
        d = res.x;
        float fogAmount = 0.0025;
        float lightDis = -1.0;
        vec3 light = evaluateLight(p, lightDis);
        d = min(min(d, 1.0), max(lightDis, 0.05));
        vec3 lightIntegrated = light - light * exp(-fogAmount * d);
        scatteredLight += transmittance * lightIntegrated;
        transmittance *= exp(-fogAmount * d);

        t += d;
        float m = res.y;
        bool end = i == maxIter - 1 ||t > maxDis;
        if (d < 0.01 || end) {
        vec3 c = vec3(1);
        vec3 normal = getNormal(p);
        if (m == MAT_SPIN) {
          float angle = atan(p.y, p.x);
          float numParts = 12.0;
          float partSize = PI / numParts;
          angle += PI;
          float part = floor((angle + TIME * 0.2) / partSize);
          float len = length(p.xy);
          float lenSize = 7.0;
          float lenY = mod(len, lenSize) - lenSize * 0.5;
          float lenPart = floor(len / lenSize);
           part = part + lenPart * 10.0;
           vec3 newPos = vec3(len * cos(angle), lenY * sin(angle), p.z);
          c = vec3(0.4 + 0.4 * sin(part), 0.4 + 0.4 * sin(part * 0.5 + 1.0), 0.4 + 0.4 * sin(part * 0.25 + 2.0));
          c*=3.0;

        } else if (m == MAT_GROUND) {
          c = vec3(0.5);
        } else if (m == MAT_PILLAR) {
          c = vec3(1);
        }
        c *= occlusion(p, normal);
        addLightning(c, normal, eye, p);
        if (end) {
          transmittance = 0.0;
        }
        col = mix(col, transmittance * c + scatteredLight, ref);
        if (m == MAT_PILLAR && p.z < -0.0 && p.z > -9.0) {
          ref = 1.0;
        } else {
          ref = 0.0;
        }
        rd = reflect(rd, getNormal(p));
        ro = p + rd*0.02;
        t = 0.0;
        break;
      }
      if (t > maxDis) {
        break;
      }
    }

    if (ref < 0.1) {
      break;
    }
  }
  finalPos = p;
  return col;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
  float u = (fragCoord.x / iResolution.x) * 2.0 - 1.0;
    float v = ((fragCoord.y / iResolution.y) * 2.0 - 1.0) * (9.0/ 16.0);//* (iResolution.y/iResolution.x);
    
    float t = TIME;
    vec3 eye = vec3(0.0, -20.0 - TIME*2.0 , -30.0 + TIME);
  vec3 tar = vec3(0, -15, 0); 
  vec3 lol = vec3(0, 1, 0);
  if (t > 20.0) {
    eye = vec3(3.0, 3.0 , 40.0 - (t + 4.0) * 2.3);
    tar = vec3(2, 0, 0); 
    lol = vec3(0,0,-1);
    eye.xy = vec2(3,3) + smoothstep(30.0,30.8, t) * (vec2(0) - vec2(3,3));
  } else if (t > 17.0) {
    eye = vec3(5.0 - t, 3.0 + t , -20.0);
    tar = eye + vec3(0,0.1, 1); 
    lol = vec3(0,0,-1);
  } else if (t > 8.0) {
    eye = vec3(15.0 - t, 13.0 , -12.0);
    tar = vec3(2, 0, 0); 
    lol = vec3(0,0,-1);
  }
  vec3 dir = normalize(tar - eye);
  vec3 right = normalize(cross(lol, dir));
  vec3 up = cross(dir, right);

  vec3 ro = eye;
  vec3 rd = normalize(dir + right*u + up*v);

  vec3 light = vec3(0.0, 0.0, 26.0 );

  vec3 finalPos = vec3(-1.0, -1.0, -1.0);
  float material = -1.0;
  vec3 color = raymarch(ro, rd, finalPos, eye);

    fragColor = vec4(color, 1.0);
    fragColor.rgb = fragColor.rgb / (fragColor.rgb + vec3(1.0));
}
